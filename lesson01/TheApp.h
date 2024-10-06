/**
 * ============================================================================
 *  Name        : TheApp.h
 *  Part of     : Simple OpenGL graphics engine framework
 *  Description : the concrete application layer
 *  Version     : 1.00
 *	Author		: Jani Immonen, <realdashdev@gmail.com>
 * ============================================================================
 **/

#pragma once

#include "../core/include/IApplication.h"
#include "../core/include/OpenGLRenderer.h"

struct VERTEX
{

	VERTEX()
	{
		x, y, z = 0.0f;
		// tu, tv = 0.0f;
		r, g, b = 1.0f;
	}

	VERTEX(float _x, float _y, float _z)
	{
		x = _x;
		y = _y;
		z = _z;
		// tu, tv = 0.0f;
		r, g, b = 1.0f;
	}

	// VERTEX(float _x, float _y, float _z, float _tu, float _tv)
	// {
	// 	x = _x;
	// 	y = _y;
	// 	z = _z;
	// 	tu = _tu;
	// 	tv = _tv;
	// 	r, g, b = 0.0f;
	// }

	VERTEX(float _x, float _y, float _z, float _r, float _g, float _b)
	{
		x = _x;
		y = _y;
		z = _z;
		r = _r;
		g = _g;
		b = _b;
	}

	// static constexpr int32_t GetStride() { return 3 * sizeof(float); }
	// static constexpr int32_t GetStride() { return 20; }
	static constexpr int32_t GetStride() { return 24; }

	void Rotate(glm::mat4 rotationMat){
		glm::vec4 vertex = glm::vec4(x, y, z, 1.0f);
		glm::vec4 newVertex = rotationMat * vertex;
		x = newVertex[0];
		y = newVertex[1];
		z = newVertex[2];
	}

	void Translate(glm::mat4 translationMat){
		glm::vec4 vertex = glm::vec4(x, y, z, 1.0f);
		glm::vec4 newVertex = translationMat * vertex;
		x = newVertex[0];
		y = newVertex[1];
		z = newVertex[2];
	}

	float x, y, z;
	// float tu, tv;
	float r, g, b;
};

class TheApp : public IApplication
{
public:
	TheApp();

	/**
	 * OnCreate
	 * app initializer, called by abstraction layer when engine initialization is complete
	 * @return true if successful, false otherwise.
	 */
	bool OnCreate() override;

	/**
	 * OnDestroy
	 * app destroyer, called by the abstraction layer when app is about to get destoyed
	 */
	void OnDestroy() override;

	/**
	 * OnUpdate
	 * app update loop
	 * @param fFrametime time since previous update, in seconds
	 */
	void OnUpdate(float frametime) override;

	/**
	 * OnDraw
	 * app drawing method
	 * @param renderer
	 */
	void OnDraw(IRenderer &renderer) override;

protected:
	/**
	 * OnKeyDown
	 * key down event
	 * @param keyCode key code
	 * @return true if event was handled by the receiver
	 */
	bool OnKeyDown(uint32_t keyCode) override;

private:
	/**
	 * GetOpenGLRenderer
	 * helper method to access the openGL renderer layer
	 * @return pointer to OpenGL renderer
	 */
	OpenGLRenderer *GetOpenGLRenderer() { return static_cast<OpenGLRenderer *>(GetRenderer()); }

private:
	// app data
	VERTEX			m_Quad[15];
	// VERTEX			Triangle[3];

	GLuint			m_uVertexShader;
	GLuint			m_uFragmentShader;
	GLuint			m_uProgram;

	GLuint			m_uTexture;

	glm::mat4		m_mModel;
	glm::mat4		m_mView;
	glm::mat4		m_mProjection;

	float			m_fX;
	float			m_fAngle;
};
