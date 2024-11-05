/**
 * ============================================================================
 *  Name        : TheApp.cpp
 *  Part of     : Simple OpenGL graphics engine framework
 *  Description : the concrete application layer
 *  Version     : 1.00
 *	Author		: Jani Immonen, <realdashdev@gmail.com>
 * ============================================================================
**/

#include "TheApp.h"


// constructor, init members
TheApp::TheApp() :
	m_uVertexShader(0),
	m_uFragmentShader(0),
	m_uProgram(0)
{
	// seed the random number generator
	RandSeed();
}


bool TheApp::OnCreate()
{
	// OnCreate is called by the application when window and graphics initialization is complete
	auto renderer = GetOpenGLRenderer();
	m_uVertexShader = renderer->CreateVertexShaderFromFile("multitexturing.vert");
	m_uFragmentShader = renderer->CreateFragmentShaderFromFile("multitexturing.frag");
	m_uProgram = renderer->CreateProgram(m_uVertexShader, m_uFragmentShader);
	if (!m_uVertexShader || !m_uFragmentShader || !m_uProgram)
	{
		return false;
	}

	// setup our view and projection matrices
	const glm::mat4 view = glm::lookAt(
		glm::vec3(0.0f, 0.0f, 9.5f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));

	const glm::mat4 projection = glm::perspective(0.61f, GetAspect(), 1.0f, 500.0f);

	// set them into the renderer
	GetRenderer()->SetViewMatrix(view);
	GetRenderer()->SetProjectionMatrix(projection);

	// generate sphere geometry
	m_pSphere = std::make_shared<Geometry>();
	m_pSphere->GenSphere(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), 24, 24);

	// generate cube geometry
	m_pCube = std::make_shared<Geometry>();
	m_pCube->GenCube(glm::vec3(3.0f, 3.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	// default material
	m_pMaterial = std::make_shared<Material>();

	// build scenegraph
	m_pSceneRoot = std::make_unique<Node>();

	// spawn a cube
	auto cube = std::make_shared<GeometryNode>(m_pCube, nullptr);
	cube->SetTextureWrapModes({ GL_CLAMP_TO_EDGE, GL_REPEAT, GL_REPEAT, GL_REPEAT });

	cube->SetName("cube");
	cube->SetPos(-2.5f, 0.0f, 0.0f);
	m_pSceneRoot->AddNode(cube);

	// spawn a sphere
	auto sphere = std::make_shared<GeometryNode>(m_pSphere, nullptr);
	sphere->SetTextureWrapModes({ GL_CLAMP_TO_EDGE, GL_REPEAT, GL_REPEAT, GL_REPEAT });
	sphere->SetName("sphere");
	sphere->SetPos(2.5f, 0.0f, 0.0f);
	m_pSceneRoot->AddNode(sphere);

	// set some parameters to all objects in scene
	for (auto& node : m_pSceneRoot->GetNodes())
	{
		auto obj = std::static_pointer_cast<GeometryNode>(node);
		const glm::vec3 axis(glm::linearRand(-1.0f, 1.0f),
			glm::linearRand(-1.0f, 1.0f),
			glm::linearRand(-1.0f, 1.0f));
		obj->SetRotationAxis(glm::normalize(axis));
		obj->SetRotationSpeed(glm::linearRand(-1.0f, 1.0f));
		obj->SetVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
		obj->SetMaterial(m_pMaterial);
	}

	return true;
}


void TheApp::OnDestroy()
{
	// app is about to close, clear all resources
	m_pSceneRoot = nullptr;

	glDeleteProgram(m_uProgram);
	glDeleteShader(m_uFragmentShader);
	glDeleteShader(m_uVertexShader);

	m_uVertexShader = 0;
	m_uFragmentShader = 0;
	m_uProgram = 0;
}


void TheApp::OnUpdate(float frametime)
{
	if (m_pSceneRoot)
	{
		m_pSceneRoot->Update(frametime);
	}
}


void TheApp::OnDraw(IRenderer& renderer)
{
	// clear depth and stencil buffers
	renderer.Clear(0.2f, 0.2f, 0.2f, 1.0f);

	// activate our program
	glUseProgram(m_uProgram);

	// setup the light direction
	const glm::vec3 lightDir(0.0f, 0.0f, -1.0f);
	OpenGLRenderer::SetUniformVec3(m_uProgram, "lightDirection", glm::normalize(lightDir));

	// setup the camera position
	const glm::vec3 campos(-renderer.GetViewMatrix()[3]);
	OpenGLRenderer::SetUniformVec3(m_uProgram, "cameraPosition", campos);

	// disable OpenGL blending as multitexturing will handle the transparency effects
	glDisable(GL_BLEND);

	if (m_pSceneRoot)
	{
		m_pSceneRoot->Render(renderer, m_uProgram);
	}
}


void TheApp::OnScreenSizeChanged(uint32_t widthPixels, uint32_t heightPixels)
{
	const glm::mat4 projection = glm::perspective(0.61f, GetAspect(), 1.0f, 500.0f);
	GetRenderer()->SetProjectionMatrix(projection);
}


bool TheApp::OnKeyDown(uint32_t keyCode)
{
	return false;
}

