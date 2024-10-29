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
	m_uVertexShader = renderer->CreateVertexShaderFromFile("phongshader.vert");
	m_uFragmentShader = renderer->CreateFragmentShaderFromFile("phongshader.frag");
	m_uProgram = renderer->CreateProgram(m_uVertexShader, m_uFragmentShader);
	m_uTexture = renderer->CreateTexture("earth.jpg");
	if (!m_uVertexShader || !m_uFragmentShader || !m_uProgram || !m_uTexture)
	{
		return false;
	}

	// setup our view and projection matrices
	const glm::mat4 view = glm::lookAt(
		glm::vec3(0.0f, 0.0f, 32.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));

	const glm::mat4 projection = glm::perspective(0.61f, GetAspect(), 1.0f, 500.0f);

	// set them into the renderer
	GetRenderer()->SetViewMatrix(view);
	GetRenderer()->SetProjectionMatrix(projection);

	// generate geometry
	constexpr float radius = 2.0f;
	m_pGeometry = std::make_shared<Geometry>();
	m_pGeometry->GenSphere(glm::vec3(radius));

	// setup the material
	m_pMaterial = std::make_shared<Material>();
	m_pMaterial->m_cAmbient = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
	m_pMaterial->m_cDiffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	m_pMaterial->m_cEmissive = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	m_pMaterial->m_cSpecular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	m_pMaterial->m_fSpecularPower = 25.0f;

	// build the scene
	m_pSceneRoot = std::make_unique<Node>();

	for (size_t i = 0; i < 25; i++)
	{
		auto node = std::make_shared<GeometryNode>(m_pGeometry, m_pMaterial);
		node->SetTexture(0, m_uTexture);
		node->SetRadius(radius);
		node->SetPos(glm::vec3(glm::linearRand(-10.0f, 10.0f), 
			glm::linearRand(-10.0f, 10.0f), 
			glm::linearRand(-10.0f, 10.0f)));

		const glm::vec3 axis(glm::linearRand(-1.0f, 1.0f),
			glm::linearRand(-1.0f, 1.0f),
			glm::linearRand(-1.0f, 1.0f));

		node->SetRotationAxis(glm::normalize(axis));
		node->SetRotationSpeed(glm::linearRand(-3.0f, 3.0f));

		node->SetVelocity(glm::vec3(glm::linearRand(-10.0f, 10.0f),
			glm::linearRand(-10.0f, 10.0f),
			glm::linearRand(-10.0f, 10.0f)));

		// add new node to the scene
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
	if (m_pSceneRoot){
		m_pSceneRoot->Update(frametime);
		CheckMovementLimits();
		CheckSphereToSphereCollisions();
	}
}


void TheApp::OnDraw(IRenderer& renderer)
{
	// clear depth and stencil buffers
	renderer.Clear(0.2f, 0.2f, 0.2f, 1.0f);

	// activate program
	glUseProgram(m_uProgram);

	// setup the light direction
	const glm::vec3 lightDirection(glm::normalize(glm::vec3(1.0f, 0.1f, -1.0f)));
	OpenGLRenderer::SetUniformVec3(m_uProgram, "lightDirection", lightDirection);

	// setup the camera position
	const glm::vec3 campos(-renderer.GetViewMatrix()[3]);
	OpenGLRenderer::SetUniformVec3(m_uProgram, "cameraPosition", campos);

	// render the scene
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

void TheApp::CheckMovementLimits()
{
	constexpr float limit = 10.0f;
	const auto& nodes = m_pSceneRoot->GetNodes();
	for (auto& node : nodes)
	{
		auto pos = node->GetPos();
		auto& velocity = node->GetVelocity();

		if (pos.x > limit)
		{
			pos.x = limit;
			velocity.x = -velocity.x;
		}
		if (pos.x < -limit)
		{
			pos.x = -limit;
			velocity.x = -velocity.x;
		}

		if (pos.y > limit)
		{
			pos.y = limit;
			velocity.y = -velocity.y;
		}
		if (pos.y < -limit)
		{
			pos.y = -limit;
			velocity.y = -velocity.y;
		}

		if (pos.z > limit)
		{
			pos.z = limit;
			velocity.z = -velocity.z;
		}
		if (pos.z < -limit)
		{
			pos.z = -limit;
			velocity.z = -velocity.z;
		}

		node->SetPos(pos);
	}
}


void TheApp::CheckSphereToSphereCollisions()
{
	const auto& nodes = m_pSceneRoot->GetNodes();

	for (auto& node1 : nodes)
	{
		for (auto& node2 : nodes)
		{
			if (node1 != node2)
			{
				glm::vec3 pos1 = node1->GetPos();
				glm::vec3 pos2 = node2->GetPos();
				glm::vec3 d(pos2 - pos1);

				const float len = glm::length(d);
				const float r = node1->GetRadius() + node2->GetRadius();

				if (len < r)
				{
					// spheres intercect

					// calculate how much object intersect
					const float inside = (r - len) * 1.01f;

					d = glm::normalize(d);
					pos1 += d * -inside * 0.5f;
					pos2 += d * inside * 0.5f;

					node1->SetPos(pos1);
					node2->SetPos(pos2);

					// rest of the collision response from gamasutra article:
					// http://www.gamasutra.com/view/feature/131424/pool_hall_lessons_fast_accurate.php
					constexpr float mass1 = 1.0f;
					constexpr float mass2 = 1.0f;

					glm::vec3 v1 = node1->GetVelocity();
					glm::vec3 v2 = node2->GetVelocity();

					float a1 = glm::dot(v1, d);
					float a2 = glm::dot(v2, d);

					float optP = (2.0f * (a1- a2)) / (mass1 + mass2);
					v1 = v1 - optP * mass2 * d;
					v2 = v2 + optP * mass1 * d;

					node1->SetVelocity(v1);
					node2->SetVelocity(v2);

					node1->SetRotationSpeed(glm::linearRand(-3.0f, 3.0f));
					node2->SetRotationSpeed(glm::linearRand(-3.0f, 3.0f));
				}
			}
		}
	}
}