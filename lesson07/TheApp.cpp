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
	m_uProgram(0),
	m_uTexture(0),
	m_fBlendAngle(0.0f)
{
	// seed the random number generator
	RandSeed();
}


bool TheApp::OnCreate()
{
	// OnCreate is called by the application when window and graphics initialization is complete
	auto renderer = GetOpenGLRenderer();
	m_uVertexShader = renderer->CreateVertexShaderFromFile("phongshader.vert");
	m_uFragmentShader = renderer->CreateFragmentShaderFromFile("phongshader.frag");
	m_uProgram = renderer->CreateProgram(m_uVertexShader, m_uFragmentShader);
	//m_uTexture = renderer->CreateTexture("white.png");
	m_uTexture = renderer->CreateTexture("hole.png");
	if (!m_uVertexShader || !m_uFragmentShader || !m_uProgram || !m_uTexture)
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

	// generate geometry
	m_pCube = std::make_shared<Geometry>();
	m_pCube->GenCube(glm::vec3(4.0f, 4.0f, 4.0f));

	// setup the materials
	m_arrMaterials.emplace_back(std::make_shared<Material>());
	m_arrMaterials.emplace_back(std::make_shared<Material>());
	m_arrMaterials.emplace_back(std::make_shared<Material>());
	m_arrMaterials[0]->m_cDiffuse = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	m_arrMaterials[1]->m_cDiffuse = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	m_arrMaterials[2]->m_cDiffuse = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

	// build scenegraph
	m_pSceneRoot = std::make_unique<Node>();

	// spawn objects
	auto object1 = std::make_shared<GeometryNode>(m_pCube, m_arrMaterials[0]);
	object1->SetPos(0.0f, 0.0f, -5.0f);

	auto object2 = std::make_shared<GeometryNode>(m_pCube, m_arrMaterials[1]);
	object2->SetPos(3.0f, 0.0f, -1.0f);

	auto object3 = std::make_shared<GeometryNode>(m_pCube, m_arrMaterials[2]);
	object3->SetPos(-3.0f, 0.0f, -1.0f);

	m_pSceneRoot->AddNode(object1);
	m_pSceneRoot->AddNode(object2);
	m_pSceneRoot->AddNode(object3);

	// set some parameters to all objects in scene
	for (auto& node : m_pSceneRoot->GetNodes())
	{
		auto gameObject = std::static_pointer_cast<GeometryNode>(node);
		//gameObject->SetRandomRotationAxis();
		gameObject->SetRotationSpeed(glm::linearRand(-1.0f, 1.0f));
	}

	return true;
}


void TheApp::OnDestroy()
{
	// app is about to close, clear all resources
	m_pSceneRoot = nullptr;

	glDeleteTextures(1, &m_uTexture);
	glDeleteProgram(m_uProgram);
	glDeleteShader(m_uFragmentShader);
	glDeleteShader(m_uVertexShader);

	m_arrMaterials.clear();

	m_uTexture = 0;
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

	m_fBlendAngle += frametime;
	m_arrMaterials[1]->m_cDiffuse.a = (cosf(m_fBlendAngle) + 1.0f) * 0.5f;
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

	// setup the diffuse texture
	renderer.SetTexture(m_uProgram, m_uTexture, 0, "texture01");
	SetTexturingParams();

	// enable blending stage
	glEnable(GL_BLEND);

	/* https://registry.khronos.org/OpenGL-Refpages/gl4/html/glBlendFunc.xhtml
	// result color = source color * source factor (+) destination color * destination factor
	// c = (0.0, 1.0, 0.0, 0.5) * 0.5 + (0.2, 0.2, 0.2, 1.0) * (1.0 - 0.5)
	// c = (0.0, 0.5, 0.0, 0.25) + (0.1, 0.1, 0.1, 0.5)
	// c = (0.1, 0.6, 0.1, 0.75)
	GL_ZERO,
	GL_ONE,
	GL_SRC_COLOR,
	GL_ONE_MINUS_SRC_COLOR,
	GL_DST_COLOR,
	GL_ONE_MINUS_DST_COLOR,
	GL_SRC_ALPHA,
	GL_ONE_MINUS_SRC_ALPHA,
	GL_DST_ALPHA,
	GL_ONE_MINUS_DST_ALPHA,
	GL_CONSTANT_COLOR,
	GL_ONE_MINUS_CONSTANT_COLOR,
	GL_CONSTANT_ALPHA,
	GL_ONE_MINUS_CONSTANT_ALPHA
	*/

	// glBlendFunc sets the source and destination factors
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// glBlend equation sets the function operation
	glBlendEquation(GL_FUNC_ADD);
	//glBlendEquation(GL_FUNC_SUBTRACT);
	//glBlendEquation(GL_FUNC_REVERSE_SUBTRACT);
	//glBlendEquation(GL_MIN);
	//glBlendEquation(GL_MAX);

	if (m_pSceneRoot)
	{
		m_pSceneRoot->Render(renderer, m_uProgram);
	}
}


void TheApp::SetTexturingParams()
{
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
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

