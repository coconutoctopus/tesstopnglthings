#pragma once
#include "GLUtil.h"

class GLShaderProgram
{
public:
	GLShaderProgram();
	~GLShaderProgram();

	int loadShaderFromFile(std::string _filename, std::string _type);

	void attachShaderSource(std::string _shaderSource, std::string _type);

	void createShaderProgram(std::string _vertexShaderSource, std::string _fragmentShaderSource);
	//----------------------------------------------------------------------------------------------------------------------
	/// @brief handler pointing to GLProgram
	//----------------------------------------------------------------------------------------------------------------------
	GLuint m_programID;
	//----------------------------------------------------------------------------------------------------------------------
	/// @brief a map of shader Programs using name as key to shader pointer
	//----------------------------------------------------------------------------------------------------------------------
	std::unordered_map <std::string, int *> m_Shaders;
};

