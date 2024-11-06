/**
 * ============================================================================
 *  Name        : TheApp.cpp
 *  Part of     : Simple OpenGL graphics engine framework
 *  Description : LAB4
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

	// TODO: load the textures
	m_arrTextures.push_back(renderer->CreateTexture("box.png"));
	m_arrTextures.push_back(renderer->CreateTexture("box_specular.png"));

	for (const auto texture : m_arrTextures)
	{
		if (!texture) return false;
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

	// TODO: generate box geometry
	m_pCube = std::make_shared<Geometry>();
	m_pCube->GenCube(glm::vec3(3.0f, 3.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	// default material
	m_pMaterial = std::make_shared<Material>();

	// build scenegraph
	m_pSceneRoot = std::make_unique<Node>();

	// spawn a cube
	auto cube = std::make_shared<GeometryNode>(m_pCube, nullptr);
	// TODO: set the textures to slot 0 and slot 1
	cube->SetTexture(0, m_arrTextures[0]);
	cube->SetTexture(1, m_arrTextures[1]);
	cube->SetName("cube");
	m_pSceneRoot->AddNode(cube);

	// rotation
	const glm::vec3 axis(glm::linearRand(-1.0f, 1.0f),
		glm::linearRand(-1.0f, 1.0f),
		glm::linearRand(-1.0f, 1.0f));
	cube->SetRotationAxis(glm::normalize(axis));
	cube->SetRotationSpeed(glm::linearRand(-1.0f, 1.0f));
	cube->SetVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
	// TODO: set material uniforms to program
	cube->SetMaterial(m_pMaterial);

	return true;
}


void TheApp::OnDestroy()
{
	// app is about to close, clear all resources
	m_pSceneRoot = nullptr;

	glDeleteTextures((GLsizei)m_arrTextures.size(), m_arrTextures.data());
	m_arrTextures.clear();

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

