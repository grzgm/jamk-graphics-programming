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
	m_uTexture(0)
{
	// seed the random number generator
	RandSeed();
}


bool TheApp::OnCreate()
{
	// OnCreate is called by the application when window and graphics initialization is complete
	auto renderer = GetOpenGLRenderer();
	m_uVertexShader = renderer->CreateVertexShaderFromFile("simpleshader.vert");
	m_uFragmentShader = renderer->CreateFragmentShaderFromFile("simpleshader.frag");
	m_uProgram = renderer->CreateProgram(m_uVertexShader, m_uFragmentShader);
	m_uTexture = renderer->CreateTexture("dice.png");
	if (!m_uVertexShader || !m_uFragmentShader || !m_uProgram || !m_uTexture)
	{
		return false;
	}

	// declare the vertices for one quad
	constexpr float hw = 0.55f;
	m_Quad[0] = VERTEX(-hw,  hw, 0.0f,		0.0f, 0.0f);
	m_Quad[1] = VERTEX( hw,  hw, 0.0f,		1.0f, 0.0f);
	m_Quad[2] = VERTEX( hw, -hw, 0.0f,		1.0f, 1.0f);

	m_Quad[3] = VERTEX(-hw,  hw, 0.0f,		0.0f, 0.0f);
	m_Quad[4] = VERTEX( hw, -hw, 0.0f,		1.0f, 1.0f);
	m_Quad[5] = VERTEX(-hw, -hw, 0.0f,		0.0f, 1.0f);

	// setup our view and projection matrices
	renderer->SetViewMatrix(glm::lookAt(
		glm::vec3(0.0f, 0.0f, 15.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f)));
	renderer->SetProjectionMatrix(glm::perspective(1.51f, GetAspect(), 0.1f, 500.0f));

	// build scenegraph
	m_pSceneRoot = std::make_unique<Node>();

	m_pSceneRoot->SetRotationSpeed(1.0f);
	m_pSceneRoot->SetVelocity(glm::vec3(0.0f, 0.0f, 1.0f));

	constexpr int32_t count = 1500;

	for(int32_t i = 0; i < count; i++){
		auto node = std::make_shared<QuadNode>();

		node->SetPos(glm::linearRand(-10.0f, 10.0f), glm::linearRand(-10.0f, 10.0f), (i * -0.01f));

		if (glm::linearRand(0, 3) == 1)
		{
			node->SetRotationSpeed(glm::linearRand(-1.0f, 1.0f));

			for (int32_t j=0; j < 5; j++)
			{
				auto n2 = std::make_shared<QuadNode>();
				n2->SetPos(glm::vec3(0.0f, j* 0.5f, 0.0f));
				node->AddNode(n2);
			}
		}

		// add new node to the scene root.
		m_pSceneRoot->AddNode(node);
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

		glm::vec3 pos = m_pSceneRoot->GetPos();
		auto& vel = m_pSceneRoot->GetVelocity();

		if (pos.z > 5.0f){
			pos.z = 5.0f;
			m_pSceneRoot->SetPos(pos);
			vel = -vel;
		}
		else if (pos.z < -5.0f) {
			pos.z = -5.0f;
			m_pSceneRoot->SetPos(pos);
			vel = -vel;
		}
	}
}


void TheApp::OnDraw(IRenderer& renderer)
{
	// clear color, depth and stencil buffers
	renderer.Clear(0.2f, 0.2f, 0.2f, 1.0f);

	QuadNode::Setup(renderer, m_uProgram, m_uTexture, (float*)m_Quad, VERTEX::GetStride());

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

	if (m_pSceneRoot){
		m_pSceneRoot->Render(renderer, m_uProgram);
	}
}


void TheApp::OnScreenSizeChanged(uint32_t widthPixels, uint32_t heightPixels)
{
	GetRenderer()->SetProjectionMatrix(glm::perspective(1.51f, GetAspect(), 0.1f, 500.0f));
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