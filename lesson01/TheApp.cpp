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
#include "misc.h"

// constructor, init members
TheApp::TheApp()
{
	m_uVertexShader = 0;
	m_uFragmentShader = 0;
	m_uProgram = 0;
	m_uTexture = 0;
	m_fX = 0;
	m_fAngle = 0;
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

	constexpr float hw = 0.55f;

	// Q: WHY DOES IT HAS TO BE CLOCKWISE????
	// m_Quad[0] = VERTEX(-hw, hw, 0.0f, 0.0f, 0.0f);
	// m_Quad[1] = VERTEX(hw, hw, 0.0f, 1.0f, 0.0f);
	// m_Quad[2] = VERTEX(hw, -hw, 0.0f, 1.0f, 1.0f);

	// m_Quad[3] = VERTEX(-hw, hw, 0.0f, 0.0f, 0.0f);
	// m_Quad[4] = VERTEX(hw, -hw, 0.0f, 1.0f, 1.0f);
	// m_Quad[5] = VERTEX(-hw, -hw, 0.0f, 0.0f, 1.0f);

	VERTEX whiteVertexTetrahedron = VERTEX(0.0f, 0.45f, 0.0f, 1.0f, 1.0f, 1.0f);
	VERTEX greenVertexTetrahedron = VERTEX(-0.4f, -0.3f, 0.3f, 0.0f, 1.0f, 0.0f);
	VERTEX blueVertexTetrahedron = VERTEX(0.4f, -0.3f, 0.3f, 0.0f, 0.0f, 1.0f);
	VERTEX redVertexTetrahedron = VERTEX(0.0f, -0.3f, -0.45f, 1.0f, 0.0f, 0.0f);
	
	m_Quad[0] = whiteVertexTetrahedron;
	m_Quad[1] = blueVertexTetrahedron;
	m_Quad[2] = greenVertexTetrahedron;
	
	m_Quad[3] = whiteVertexTetrahedron;
	m_Quad[4] = greenVertexTetrahedron;
	m_Quad[5] = redVertexTetrahedron;
	
	m_Quad[6] = redVertexTetrahedron;
	m_Quad[7] = blueVertexTetrahedron;
	m_Quad[8] = whiteVertexTetrahedron;
	
	m_Quad[9] = greenVertexTetrahedron;
	m_Quad[10] = blueVertexTetrahedron;
	m_Quad[11] =redVertexTetrahedron;

	
	// VERTEX redVertexTriangle = VERTEX(0.0f, 0.25f, 0.0f, 1.0f, 0.0f, 0.0f);
	// VERTEX greenVertexTriangle = VERTEX(-0.3f, -0.3f, 0.0f, 0.0f, 1.0f, 0.0f);
	// VERTEX blueVertexTriangle = VERTEX(0.3f, -0.3f, 0.0f, 0.0f, 0.0f, 1.0f);

	// m_Quad[12] = redVertexTriangle;
	// m_Quad[13] = blueVertexTriangle;
	// m_Quad[14] = greenVertexTriangle;

	// m_Quad[12].Translate(glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.0f)));
	// m_Quad[13].Translate(glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.0f)));
	// m_Quad[14].Translate(glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.0f)));

	// setup view and projection matrice
	m_mView = glm::lookAt(
		glm::vec3(0.0f, 0.0f, 2.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));

	m_mProjection = glm::perspective(
		1.51f,
		GetAspect(),
		0.1f,
		500.0f);

	// return false to quit the app
	return true;
}

void TheApp::OnDestroy()
{
	// app is about to close, clear all resources
	glDeleteTextures(1, &m_uTexture);
	glDeleteProgram(m_uProgram);
	glDeleteShader(m_uFragmentShader);
	glDeleteShader(m_uVertexShader);
}

void TheApp::OnUpdate(float frametime)
{
	// the main loop
	// glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), -1.57079633f, glm::vec3(1.0f, 0.0f, 0.0f));
	// rotation = glm::rotate(rotation, m_fAngle, glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), m_fAngle, glm::vec3(0.0f, 1.0f, 0.0f));
	// printMat(rotation);
	m_fAngle += frametime;

	m_fX += frametime * 0.5f;
	if (m_fX > 5.0f)
	{
		m_fX = -5.0f;
	}

	// m_mModel = glm::mat4(1.0f);
	// m_mModel[3][1] = m_fX;
	glm::mat4 translation(1.0f);
	// translation[3][0] = -1.5f;
	// translation[3][1] = -1.0f;

	m_mModel = translation * rotation;
	// m_mModel = rotation;
	// m_mModel = glm::mat4(1.0f);

	m_mView[3][1] = 0.0f + sinf(m_fAngle) * 1.0f;
	m_mView[3][0] = 0.0f + sinf(m_fAngle * 3) * 0.1f;

	// for (int i = 0; i < 15; i++)
	// {
	// 	m_Quad[i].Rotate(glm::rotate(glm::mat4(1.0f), 0.05f, glm::vec3(0.0f, 1.0f, 0.0f)));
	// }
}

void TheApp::OnDraw(IRenderer &renderer)
{
	// clear color, depth and stencil buffers
	renderer.Clear(0.1f, 0.5f, 0.1f, 1.0f);

	// setup the rendering with OpenGL
	glUseProgram(m_uProgram);

	GLint position = glGetAttribLocation(m_uProgram, "position");
	glEnableVertexAttribArray(position);
	glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, VERTEX::GetStride(), m_Quad);

	GLint rgb = glGetAttribLocation(m_uProgram, "rgb");
	glEnableVertexAttribArray(rgb);
	glVertexAttribPointer(rgb, 3, GL_FLOAT, GL_FALSE, VERTEX::GetStride(), (float*)m_Quad + 3);

	// GLint uv = glGetAttribLocation(m_uProgram, "uv");
	// glEnableVertexAttribArray(uv);
	// glVertexAttribPointer(uv, 2, GL_FLOAT, GL_FALSE, VERTEX::GetStride(), (float*)m_Quad + 3);

	// renderer.SetTexture(m_uProgram, m_uTexture, 0, "texture01");

	// set the program uniforms
	glm::mat4 mvp = m_mProjection * m_mView * m_mModel;
	GetOpenGLRenderer()->SetUniformMatrix4(m_uProgram, "mvpMatrix", mvp);

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

	// draw quad
	glDrawArrays(GL_TRIANGLES, 0, 12);
}

bool TheApp::OnKeyDown(uint32_t keyCode)
{
	// if (keyCode == KEY_ESC)
	// {
	// 	Close();
	// 	return true;
	// }

	return false;
}
