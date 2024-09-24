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
TheApp::TheApp()
{
	m_uVertexShader = 0;
	m_uFragmentShader = 0;
	m_uProgram = 0;
	m_fAngle = 0;
	stopAnimation = true;
	// seed the random number generator
	RandSeed();
}


bool TheApp::OnCreate()
{
	// OnCreate is called by the application when window and graphics initialization is complete
	auto renderer = GetOpenGLRenderer();
	m_uVertexShader = renderer->CreateVertexShaderFromFile("triangleshader.vert");
	m_uFragmentShader = renderer->CreateFragmentShaderFromFile("triangleshader.frag");
	m_uProgram = renderer->CreateProgram(m_uVertexShader, m_uFragmentShader);
	if (!m_uVertexShader || !m_uFragmentShader || !m_uProgram)
	{
		return false;
	}

	// TODO: make a gouraud shaded triangle
	VERTEX redVertexTetrahedron = VERTEX(0.5f, -0.4f, 0.0f, 1.0f, 0.0f, 0.0f);
	VERTEX greenVertexTetrahedron = VERTEX(-0.5f, -0.4f, 0.0f, 0.0f, 1.0f, 0.0f);
	VERTEX blueVertexTetrahedron = VERTEX(0.0f, 0.65f, 0.0f, 0.0f, 0.0f, 1.0f);

	triangle[0] = blueVertexTetrahedron;
	triangle[1] = redVertexTetrahedron;
	triangle[2] = greenVertexTetrahedron;

	// setup view and projection matrice
	m_mView = glm::lookAt(
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));

	m_mProjection = glm::perspective(
		1.51f,
		GetAspect(),
		0.1f,
		500.0f);

	return true;
}


void TheApp::OnDestroy()
{
	// app is about to close, clear all resources
	glDeleteProgram(m_uProgram);
	glDeleteShader(m_uFragmentShader);
	glDeleteShader(m_uVertexShader);

	m_uVertexShader = 0;
	m_uFragmentShader = 0;
	m_uProgram = 0;
}


void TheApp::OnUpdate(float frametime)
{
	if(!stopAnimation)
	{
		// rotation for model matrix
		// glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), m_fAngle, glm::vec3(0.0f, 0.0f, 1.0f));
		// m_fAngle += frametime;

		// m_mModel = rotation;

		// in object rotation
		m_mModel = glm::mat4(1.0f);

		for (int i = 0; i < 3; i++)
		{
			triangle[i].Rotate(glm::rotate(glm::mat4(1.0f), frametime, glm::vec3(0.0f, 0.0f, 1.0f)));
		}
	}
}


void TheApp::OnDraw(IRenderer& renderer)
{
	// clear color, depth and stencil buffers
	renderer.Clear(0.2f, 0.2f, 0.2f, 1.0f);

	// setup the rendering program
	glUseProgram(m_uProgram);

	GLint position = glGetAttribLocation(m_uProgram, "position");
	glEnableVertexAttribArray(position);
	glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, VERTEX::GetStride(), triangle);

	GLint rgb = glGetAttribLocation(m_uProgram, "rgb");
	glEnableVertexAttribArray(rgb);
	glVertexAttribPointer(rgb, 3, GL_FLOAT, GL_FALSE, VERTEX::GetStride(), (float*)triangle + 3);

	// Simple way or mvp
	// glm::mat4 mvp = m_mProjection * m_mView * m_mModel;
	glm::mat4 mvp = m_mModel;

	GetOpenGLRenderer()->SetUniformMatrix4(m_uProgram, "mvpMatrix", mvp);

	glDrawArrays(GL_TRIANGLES, 0, 3);
}


void TheApp::OnScreenSizeChanged(uint32_t widthPixels, uint32_t heightPixels)
{
}


bool TheApp::OnKeyDown(uint32_t keyCode)
{
	if (keyCode == KEY_ESC)
	{
		Close();
		return true;
	}

	if (keyCode == KEY_SPACE)
	{
		stopAnimation = !stopAnimation;
	}

	return false;
}

