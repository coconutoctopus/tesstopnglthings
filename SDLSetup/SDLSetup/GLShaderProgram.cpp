#include "GLShaderProgram.h"

GLShaderProgram::GLShaderProgram()
{
	GLuint m_programID = glCreateProgram();
}

GLShaderProgram::~GLShaderProgram()
{
}

int GLShaderProgram::loadShaderFromFile(std::string _filename, std::string _type)
{
	// check for shader compile errors
	int success;
	char infoLog[512];

	std::string shaderString;
	int newShader;

	//
	std::ifstream shaderSource(_filename.c_str());

	//If it doesn't open the source, it doesn't exist
	if (!shaderSource.is_open())
	{
		std::cerr << "File not found " << _filename.c_str() << std::endl;
		return -1;
	}

	//Else read in the shader source
	shaderString = std::string((std::istreambuf_iterator<char>(shaderSource)), std::istreambuf_iterator<char>());
	shaderSource.close();

	shaderString += '\0';
	const char* data = shaderString.c_str();

	if (_type == "vertex")
	{
		newShader = glCreateShader(GL_VERTEX_SHADER);
	}
	else
	{
		newShader = glCreateShader(GL_FRAGMENT_SHADER);
	}

	glShaderSource(newShader, 1, &data, NULL);
	glCompileShader(newShader);

	glGetShaderiv(newShader, GL_COMPILE_STATUS, &success);
	
	if (!success)
	{
		glGetShaderInfoLog(newShader, 512, NULL, infoLog);
		std::cout << "SHADER "<< _filename << " FAILED TO COMPILE" << infoLog << std::endl;
	}

	return newShader;
}

void GLShaderProgram::attachShaderSource(std::string _shaderSource, std::string _type)
{

}

void GLShaderProgram::createShaderProgram(std::string _vertexShaderSource, std::string _fragmentShaderSource)
{
	// check for shader compile errors
	int success;
	char infoLog[512];

	int shaderProgram = glCreateProgram();
	m_programID = shaderProgram;

	std::cout << "PROGRAM CREATED" << std::endl;
	int vertexShader = loadShaderFromFile(_vertexShaderSource, "vertex");
	int fragmentShader = loadShaderFromFile(_fragmentShaderSource, "fragment");

	glAttachShader(m_programID, vertexShader);
	glAttachShader(m_programID, fragmentShader);
	glLinkProgram(m_programID);

	glGetProgramiv(m_programID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(m_programID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING::FAILED " << infoLog << std::endl;
	}

	//After linking the shaders to the program, we can delete them.
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

}
