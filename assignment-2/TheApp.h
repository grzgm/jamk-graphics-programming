/**
 * ============================================================================
 *  Name        : TheApp.h
 *  Part of     : Simple OpenGL graphics engine framework
 *  Description : LAB2
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
		tu, tv = 0.0f;
	}

	VERTEX(float _x, float _y, float _z)
	{
		x = _x;
		y = _y;
		z = _z;
		tu, tv = 0.0f;
	}

	VERTEX(float _x, float _y, float _z, float _tu, float _tv)
	{
		x = _x;
		y = _y;
		z = _z;
		tu = _tu;
		tv = _tv;
	}

	static constexpr int32_t GetStride() { return 20; }

	float x, y, z;
	float tu, tv;
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
	 * @param frametime time since previous update, in seconds
	 */
	void OnUpdate(float frametime) override;

	/**
	 * OnDraw
	 * app drawing method
	 * @param pRenderer pointer to renderer object
	 */
	void OnDraw(IRenderer& renderer) override;

protected:
	/**
	 * OnScreenSizeChanged
	 * event when screen size changes
	 * @param uWidthPixels, uHeightPixels new pixel size of the screen
	 */
	void OnScreenSizeChanged(uint32_t widthPixels, uint32_t heightPixels) override;

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
	OpenGLRenderer* GetOpenGLRenderer() { return static_cast<OpenGLRenderer*>(GetRenderer()); }

private:
	// app data
	GLuint			m_uVertexShader;
	GLuint			m_uFragmentShader;
	GLuint			m_uProgram;

	VERTEX			triangle[3];

	GLuint			m_uTexture;

	glm::mat4		m_mModel;
	glm::mat4		m_mView;
	glm::mat4		m_mProjection;

	bool			flipSignal;
	float			range;
	float			invert;
};

