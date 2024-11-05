/**
 * ============================================================================
 *  Name        : TheApp.cpp
 *  Part of     : Simple OpenGL graphics engine framework
 *  Description : LAB2
 *  Version     : 1.00
 *	Author		: Jani Immonen, <realdashdev@gmail.com>
 * ============================================================================
 **/

#include "TheApp.h"

// constructor, init members
TheApp::TheApp() : m_uVertexShader(0),
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
	m_uTexture = renderer->CreateTexture("white.png");
	if (!m_uVertexShader || !m_uFragmentShader || !m_uProgram || !m_uTexture)
	{
		return false;
	}

	// setup our view and projection matrices
	m_mView = glm::lookAt(
		glm::vec3(0.0f, 7.0f, 25.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));
	m_mProjection = glm::perspective(0.7f, GetAspect(), 0.1f, 500.0f);

	// build the scenegraph
	m_pSceneRoot = std::make_unique<Node>();

	// TODO: build a tank

	std::shared_ptr<Material> tankMaterial = std::make_shared<Material>();
	// tankMaterial->m_cAmbient = glm::vec4(0.1f, 1.0f, 0.1f, 1.0f);
	tankMaterial->m_cDiffuse = glm::vec4(1.0f, 0.1f, 0.1f, 1.0f);
	// tankMaterial->m_cEmissive = glm::vec4(1.0f, 0.1f, 0.1f, 1.0f);
	// tankMaterial->m_cSpecular = glm::vec4(1.0f, 0.1f, 0.1f, 1.0f);
	// tankMaterial->m_fSpecularPower = 25.0f;

	// Base of the tank
	std::shared_ptr<Geometry> mTankBaseGeometry = std::make_shared<Geometry>();
	mTankBaseGeometry->GenCube(glm::vec3(5.0f, 2.0f, 6.0f));

	mTankBase = std::make_shared<GeometryNode>(mTankBaseGeometry, tankMaterial);

	// Turret of the tank
	std::shared_ptr<Geometry> mTankTurretGeometry = std::make_shared<Geometry>();
	mTankTurretGeometry->GenCube(glm::vec3(3.0f, 2.0f, 3.0f));

	mTankTurret = std::make_shared<GeometryNode>(mTankTurretGeometry, tankMaterial);
	mTankTurret->SetPos(glm::vec3(0.0f, 2.0f, 0.0f));

	// Pipe of the tank
	std::shared_ptr<Geometry> mTankPipeGeometry = std::make_shared<Geometry>();
	mTankPipeGeometry->GenCube(glm::vec3(1.0f, 1.0f, 3.5f));

	mTankPipe = std::make_shared<GeometryNode>(mTankPipeGeometry, tankMaterial);
	mTankPipe->SetPos(glm::vec3(0.0f, 0.0f, -2.5f));

	mTankTurret->AddNode(mTankPipe);
	mTankBase->AddNode(mTankTurret);
	m_pSceneRoot->AddNode(mTankBase);

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
	// update scenegraph
	if (m_pSceneRoot)
	{
		m_pSceneRoot->Update(frametime);

		// Tank Base Rotation
		if (IsKeyDown(KEY_LEFT))
		{
			const glm::vec3 axis(0.0f, 1.0f, 0.0f);

			mTankBase->SetRotationAxis(glm::normalize(axis));
			mTankBase->SetRotationSpeed(1.0f);
		}
		else if (IsKeyDown(KEY_RIGHT))
		{
			const glm::vec3 axis(0.0f, 1.0f, 0.0f);

			mTankBase->SetRotationAxis(glm::normalize(axis));
			mTankBase->SetRotationSpeed(-1.0f);
		}
		else
		{
			const glm::vec3 axis(0.0f, 1.0f, 0.0f);
			mTankBase->SetRotationAxis(glm::normalize(axis));
			mTankBase->SetRotationSpeed(0.0f);
		}

		// Tank Base Movement
		if (IsKeyDown(KEY_UP))
		{
			// Tank Base Movement forward given rotation
			glm::mat4 modelMatrix = mTankBase->GetMatrix();
			glm::vec3 currentModelMatrix = glm::normalize(modelMatrix[2]);
			glm::vec3 offset = currentModelMatrix * -0.1f;
			glm::vec3 currentPosition = glm::vec3(modelMatrix[3]);
			glm::vec3 newPosition = currentPosition + offset;
			modelMatrix[3] = glm::vec4(newPosition, 1.0f);
			mTankBase->SetMatrix(modelMatrix);
		}
		else if (IsKeyDown(KEY_DOWN))
		{
			// Tank Base Movement backwards given rotation
			glm::mat4 modelMatrix = mTankBase->GetMatrix();
			glm::vec3 currentModelMatrix = glm::normalize(modelMatrix[2]);
			glm::vec3 offset = currentModelMatrix * 0.1f;
			glm::vec3 currentPosition = glm::vec3(modelMatrix[3]);
			glm::vec3 newPosition = currentPosition + offset;
			modelMatrix[3] = glm::vec4(newPosition, 1.0f);
			mTankBase->SetMatrix(modelMatrix);
		}

		// Tank Turret Rotation
		if (IsKeyDown('A'))
		{
			const glm::vec3 axis(0.0f, 1.0f, 0.0f);

			float currentRotation = mTankTurret->GetRotationAngle();
			mTankTurret->RotateAxisAngle(glm::normalize(axis), currentRotation + 0.1f);
		}
		if (IsKeyDown('D'))
		{
			const glm::vec3 axis(0.0f, 1.0f, 0.0f);

			float currentRotation = mTankTurret->GetRotationAngle();
			mTankTurret->RotateAxisAngle(glm::normalize(axis), currentRotation - 0.1f);
		}

		// Tank Pipe Rotation
		if (IsKeyDown('W'))
		{
			const glm::vec3 axis(1.0f, 0.0f, 0.0f);

			float currentRotation = mTankPipe->GetRotationAngle();
			if (currentRotation < 0.5f)
			{
				glm::vec3 mTankPipePos = mTankPipe->GetPos();
				mTankPipe->RotateAxisAngle(glm::normalize(axis), currentRotation + 0.05f);
				mTankPipePos.y += 0.07f;
				mTankPipe->SetPos(mTankPipePos);
			}
		}
		if (IsKeyDown('S'))
		{
			const glm::vec3 axis(1.0f, 0.0f, 0.0f);

			float currentRotation = mTankPipe->GetRotationAngle();
			if (currentRotation > -0.5f)
			{
				glm::vec3 mTankPipePos = mTankPipe->GetPos();
				mTankPipe->RotateAxisAngle(glm::normalize(axis), currentRotation - 0.05f);
				mTankPipePos.y -= 0.07f;
				mTankPipe->SetPos(mTankPipePos);
			}
		}
	}
}

void TheApp::OnDraw(IRenderer &renderer)
{
	// clear color, depth and stencil buffers
	renderer.Clear(0.2f, 0.2f, 0.2f, 1.0f);

	// set view and projection into the renderer
	GetRenderer()->SetViewMatrix(m_mView);
	GetRenderer()->SetProjectionMatrix(m_mProjection);

	// setup the rendering of our quad
	glUseProgram(m_uProgram);

	// set the texture for the quad (slot 0)
	renderer.SetTexture(m_uProgram, m_uTexture, 0, "texture01");

	// setup the light position (to camera position)
	const glm::vec3 campos(-renderer.GetViewMatrix()[3]);
	OpenGLRenderer::SetUniformVec3(m_uProgram, "lightPosition", campos);

	// setup the camera position uniform
	OpenGLRenderer::SetUniformVec3(m_uProgram, "cameraPosition", campos);

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
