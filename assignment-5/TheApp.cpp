/**
 * ============================================================================
 *  Name        : TheApp.cpp
 *  Part of     : Simple OpenGL graphics engine framework
 *  Description : LAB5
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
	m_uVertexShader = renderer->CreateVertexShaderFromFile("phongshader.vert");
	m_uFragmentShader = renderer->CreateFragmentShaderFromFile("phongshader.frag");
	m_uProgram = renderer->CreateProgram(m_uVertexShader, m_uFragmentShader);
	if (!m_uVertexShader || !m_uFragmentShader || !m_uProgram)
	{
		return false;
	}

	m_arrTextures.push_back(renderer->CreateTexture("grass.png"));
	m_arrTextures.push_back(renderer->CreateTexture("terrain.jpg"));

	for (const auto texture : m_arrTextures)
	{
		if (!texture) return false;
	}

	// setup our view and projection matrices
	m_mView = glm::lookAt(
		glm::vec3(0.0f, 5.0f, 30.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));

	m_mProjection = glm::perspective(0.7f, GetAspect(), 0.1f, 500.0f);

	// set view and projection into the renderer
	GetRenderer()->SetViewMatrix(m_mView);
	GetRenderer()->SetProjectionMatrix(m_mProjection);

	// TODO: generate terrain & grass geometry
	auto grassGeometry = std::make_shared<Geometry>();
	grassGeometry->GenQuad(glm::vec2(3.0f, 3.0f), glm::vec3(0.0f, 1.5f, 0.0f));

	auto terrain = std::make_shared<Geometry>();
	terrain->GenQuad(glm::vec2(50.0f, 50.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	// TODO: material
	auto material = std::make_shared<Material>();
	material->m_cDiffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	// build the scenegraph
	m_pSceneRoot = std::make_unique<Node>();
	
	// TODO: use quad as a terrain plane
	auto nodeTerrain = std::make_shared<GeometryNode>(terrain, material);
	nodeTerrain->SetName("terrain");
	nodeTerrain->SetTexture(0, m_arrTextures[1]);
	const glm::vec3 axis(1.0f, 0.0f, 0.0f);

	nodeTerrain->SetRotationAxis(glm::normalize(axis));
	nodeTerrain->SetRotationAngle(-1.5f);

	m_pSceneRoot->AddNode(nodeTerrain);

	// TODO: add grass
	for (size_t i = 0; i < 1500; i++)
	{
		auto node = std::make_shared<GeometryNode>(grassGeometry, material);
		node->SetTexture(0, m_arrTextures[0]);
		node->SetPos(glm::vec3(glm::linearRand(-23.0f, 23.0f),
			0, glm::linearRand(-23.0f, 23.0f)));

		// add new node to the scene
		m_pSceneRoot->AddNode(node);
	}

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
	// update scenegraph
    if (m_pSceneRoot)
    {
        m_pSceneRoot->Update(frametime);

        static float time = 0.0f;
        time += frametime;

        for (auto& node : (m_pSceneRoot->GetNodes()))
        {
            auto grassNode = std::dynamic_pointer_cast<GeometryNode>(node);
            if (grassNode && grassNode->GetName() != "terrain")
            {
                float shearAmount = 0.2f * glm::sin(time + grassNode->GetPos().x);

                glm::mat4 shearMatrix(1.0f);
                shearMatrix[1][0] = shearAmount;

                glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), grassNode->GetPos());
                modelMatrix *= shearMatrix;

                grassNode->SetMatrix(modelMatrix);
            }
        }
    }
}


void TheApp::OnDraw(IRenderer& renderer)
{
	// clear color, depth and stencil buffers
	renderer.Clear(0.6f, 0.6f, 1.0f, 1.0f);

	// setup the rendering program
	glUseProgram(m_uProgram);

	// setup the light position (above scene)
	const glm::vec3 campos(-renderer.GetViewMatrix()[3]);
	const glm::vec3 lightpos(0.0f, 10.0f, campos.z);

	OpenGLRenderer::SetUniformVec3(m_uProgram, "lightPosition", lightpos);

	// setup the camera position uniform
	OpenGLRenderer::SetUniformVec3(m_uProgram, "cameraPosition", campos);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquation(GL_FUNC_ADD);

	// draw the scenegraph
	if (m_pSceneRoot)
	{
		m_pSceneRoot->Render(renderer, m_uProgram);
	}
}


void TheApp::OnScreenSizeChanged(uint32_t widthPixels, uint32_t heightPixels)
{
	m_mProjection = glm::perspective(0.7f, GetAspect(), 0.1f, 500.0f);
}


bool TheApp::OnKeyDown(uint32_t keyCode)
{
	if (keyCode == KEY_ESC)
	{
		Close();
		return true;
	}

	return false;
}
