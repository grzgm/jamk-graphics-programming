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
	m_uTexture = 0;

	flipSignal = false;
	range = 1;
	invert = -1;

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

	m_uTexture = renderer->CreateTexture("leaf.png");

	if (!m_uVertexShader || !m_uFragmentShader || !m_uProgram || !m_uTexture)
	{
		return false;
	}

	VERTEX redVertexTetrahedron = VERTEX(0.5f, -0.5f, 0.0f, 1.0f, 1.0f);
	VERTEX greenVertexTetrahedron = VERTEX(-0.5f, -0.5f, 0.0f, 0.0f, 1.0f);
	VERTEX blueVertexTetrahedron = VERTEX(0.0f, 0.5f, 0.0f, 0.5f, 0.0f);

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

	glDeleteTextures(1, &m_uTexture);

	m_uVertexShader = 0;
	m_uFragmentShader = 0;
	m_uProgram = 0;
}

void TheApp::OnUpdate(float frametime)
{
	if (flipSignal)
	{
		for (auto &vertex : triangle)
		{
			vertex.tv = (vertex.tv - range) * invert;
		}
		flipSignal = false;
	}
}

void TheApp::OnDraw(IRenderer &renderer)
{
	// clear color, depth and stencil buffers
	renderer.Clear(0.2f, 0.2f, 0.2f, 1.0f);

	// setup the rendering program
	glUseProgram(m_uProgram);

	GLint position = glGetAttribLocation(m_uProgram, "position");
	glEnableVertexAttribArray(position);
	glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, VERTEX::GetStride(), triangle);

	GLint uv = glGetAttribLocation(m_uProgram, "uv");
	glEnableVertexAttribArray(uv);
	glVertexAttribPointer(uv, 2, GL_FLOAT, GL_FALSE, VERTEX::GetStride(), (float *)triangle + 3);

	renderer.SetTexture(m_uProgram, m_uTexture, 0, "texture01");

	// Simple way or mvp
	m_mModel = glm::mat4(1.0f);
	glm::mat4 mvp = m_mModel;

	GetOpenGLRenderer()->SetUniformMatrix4(m_uProgram, "mvpMatrix", mvp);

	// was not present in the demo so I have disabled it
	// glEnable(GL_BLEND);
	// glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

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
		// // simple solution, but not in the right place imo
		// for (auto &vertex : triangle)
		// {
		// 	vertex.tv = (vertex.tv - range) * invert;
		// }

		// barebone, crude and primitive event mechanism
		flipSignal = true;
	}

	return false;
}
