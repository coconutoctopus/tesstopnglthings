#pragma once

#include <iostream>

#include <SDL2\SDL.h>
#include <GL\glew.h>
#include <glm\glm.hpp>

#include "GLUtil.h"
#include "GLShaderProgram.h"
#include "GLVAO.h"

class GLScene
{
public:
	//----------------------------------------------------------------
	// @brief constructor
	//----------------------------------------------------------------
	GLScene();

	//----------------------------------------------------------------
	// @brief destructor
	//----------------------------------------------------------------
	~GLScene();

	void Initialise(int _width, int _height);


	//----------------------------------------------------------------
	// @brief resize screen and set camera 
	//----------------------------------------------------------------
	void Resize(int _width, int _height);

	void generateScreenBuffers();

	//----------------------------------------------------------------
	// @brief generate VAO with vertex information
	//----------------------------------------------------------------
	void generateBuffers();

	void DrawBuffers();

	void DrawTriangle();

	//----------------------------------------------------------------
	// @brief draw the scene
	//----------------------------------------------------------------
	void Draw();

	//----------------------------------------------------------------
	// @brief called when the mouse is moved
	//----------------------------------------------------------------
	void mouseMoveEvent(const SDL_MouseMotionEvent &_event);

	//----------------------------------------------------------------
	// @brief called when the mouse is pressed down
	//----------------------------------------------------------------
	void mousePressEvent(const SDL_MouseButtonEvent &_event);

	//----------------------------------------------------------------
	// @brief called when the mouse is pressed up
	//----------------------------------------------------------------
	void mouseReleaseEvent(const SDL_MouseButtonEvent &_event);

	//----------------------------------------------------------------
	// @brief called when the scroll wheel is moved
	//----------------------------------------------------------------
	void mouseScrollEvent(const SDL_MouseWheelEvent &_event);

private:
	//----------------------------------------------------------------
	// @brief called when the mouse is moved
	//----------------------------------------------------------------
	void loadMatricesToShader();

	//----------------------------------------------------------------
	// @brief GL Window width
	//----------------------------------------------------------------
	GLsizei m_width;

	//----------------------------------------------------------------
	// @brief GL WIndow Height
	//----------------------------------------------------------------
	GLsizei m_height;

	//----------------------------------------------------------------
	/// @brief X rotation mouse value
	//----------------------------------------------------------------
	int m_spinXFace;
	//----------------------------------------------------------------
	/// @brief Y rotation mouse value
	//----------------------------------------------------------------
	int m_spinYFace;
	//----------------------------------------------------------------
	/// @brief flag to indicate if left mouse button is pressed while moving mouse
	//----------------------------------------------------------------
	bool m_rotate;
	//----------------------------------------------------------------
	/// @brief flag to indicate if right mouse button is pressed while moving mouse
	//----------------------------------------------------------------
	bool m_translate;
	//----------------------------------------------------------------
	/// @brief X mouse value
	//----------------------------------------------------------------
	int m_origX;
	//----------------------------------------------------------------
	/// @brief Y mouse value
	//----------------------------------------------------------------
	int m_origY;
	//----------------------------------------------------------------
	/// @brief previous X mouse value
	//----------------------------------------------------------------
	int m_origXPos;
	//----------------------------------------------------------------
	/// @brief previous Y mouse value
	//----------------------------------------------------------------
	int m_origYPos;
	//----------------------------------------------------------------------------------------------------------------------
	/// @brief a map of shader Programs using name as key to shader pointer
	//----------------------------------------------------------------------------------------------------------------------
	std::unordered_map <std::string, GLShaderProgram *> m_shaderPrograms;
	//----------------------------------------------------------------------------------------------------------------------
	/// @brief a map of shader Programs using name as key to shader pointer
	//----------------------------------------------------------------------------------------------------------------------
	std::unordered_map <std::string, GLVAO *> m_vaos;
	//----------------------------------------------------------------------------------------------------------------------
	/// @brief current vao
	//----------------------------------------------------------------------------------------------------------------------
	GLVAO *m_vao;
	unsigned int VBO, VAO;
	unsigned int m_texture;
	unsigned int m_fbo;
	unsigned int m_fbTexture;
};