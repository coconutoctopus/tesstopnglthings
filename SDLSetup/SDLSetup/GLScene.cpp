#include "GLScene.h"
#include "stb_image.h"

const static float INCREMENT = 0.01f;
const static float ZOOM = 0.05f;

GLScene::GLScene()
{
	std::cout << "INITIALISE GL" << std::endl;

	GLint GlewInitResult = glewInit();
	if (GLEW_OK != GlewInitResult)
	{
		printf("ERROR: %s\n", glewGetErrorString(GlewInitResult));
		exit(EXIT_FAILURE);
	}

	m_rotate = false;

	m_spinXFace = 0;
	m_spinYFace = 0;
}

GLScene::~GLScene()
{
	std::cout << "Shutting down OpenGL" << std::endl;
}

void GLScene::Initialise(int _width, int _height)
{
	std::cout << "** INITIALISE GL **" << std::endl;

	m_width = _width;
	m_height = _height;
	glViewport(0, 0, _width, _height);

	//Start with Turquoise background! Yaay!
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	//Create programs
	GLShaderProgram *newProg = new GLShaderProgram();
	newProg->createShaderProgram("shaders/flatVertex.glsl", "shaders/flatFragment.glsl");
	m_shaderPrograms["Flat"] = newProg;
	GLShaderProgram *screenProg = new GLShaderProgram();
	screenProg->createShaderProgram("shaders/DepthBuffer.vs", "shaders/DepthBuffer.fs");
	m_shaderPrograms["Screen"] = screenProg;

	generateBuffers();
	generateScreenBuffers();
	//----------------------------------------------------------------
	// Create a Framebuffer
	//----------------------------------------------------------------
	//Framebuffers are basically of-screen rendering
	glGenFramebuffers(1, &m_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	//We need to create something to store the framebuffer info on- either a Texture or a 
	//Framebuffer object.
	// create a color attachment texture
	glGenTextures(1, &m_fbTexture);
	glBindTexture(GL_TEXTURE_2D, m_fbTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_fbTexture, 0);
	// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_width, m_height); // use a single renderbuffer object for both a depth AND stencil buffer.
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
																								  // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//----------------------------------------------------------------
	// Create a GL Texture
	//----------------------------------------------------------------
	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height, nrChannels;
	unsigned char *data = stbi_load("textures/container.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}

//----------------------------------------------------------------
// @brief resize screen and set camera 
//----------------------------------------------------------------
void GLScene::Resize(int _width, int _height)
{
	m_width = _width;
	m_height = _height;
	glViewport(0, 0, _width, _height);
}

void GLScene::generateScreenBuffers()
{
	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
							 // positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		1.0f, -1.0f,  1.0f, 0.0f,
		1.0f,  1.0f,  1.0f, 1.0f
	};

	//Vertex Buffer Objects store a bunch of information on the GPU that we can call upon when we need it.
	//Vertex Array Objects are basically an array of VBOs for the same verts.
	GLVAO *vao = new GLVAO(GL_TRIANGLES);
	vao->Bind();
	vao->setData(GL_ARRAY_BUFFER, vertices, GL_STATIC_DRAW, sizeof(vertices));
	//------------------------------------------
	//
	//------------------------------------------
	//Position Attrib
	vao->setVertexAttributePointer(0, 2, GL_FLOAT, GL_FALSE, 4, 0);
	//UV Attrib
	vao->setVertexAttributePointer(1, 2, GL_FLOAT, GL_FALSE, 4, 2);

	vao->setNumVerts(6);
	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	vao->Unbind();

	m_vaos["Screen"] = vao;
}

void GLScene::generateBuffers()
{
	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// positions         // colors			//uv coords
		0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   0.0f, 0.0f,	// bottom right
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,	1.0f, 0.0f,	// bottom left
		0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,	0.5f, 1.0f	// top 
	};

	//Vertex Buffer Objects store a bunch of information on the GPU that we can call upon when we need it.
	//Vertex Array Objects are basically an array of VBOs for the same verts.
	GLVAO *vao = new GLVAO(GL_TRIANGLES);
	vao->Bind();
	vao->setData(GL_ARRAY_BUFFER, vertices, GL_STATIC_DRAW, sizeof(vertices));
	//------------------------------------------
	//
	//------------------------------------------
	//Position Attrib
	vao->setVertexAttributePointer(0, 3, GL_FLOAT, GL_FALSE, 8, 0);
	//Colour Attrib
	vao->setVertexAttributePointer(1, 3, GL_FLOAT, GL_FALSE, 8, 3);
	//UV Attrib
	vao->setVertexAttributePointer(2, 2, GL_FLOAT, GL_FALSE, 8, 6);

	vao->setNumVerts(3);
	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	vao->Unbind();

	m_vaos["Triangle"] = vao;
}

void GLScene::DrawTriangle()
{
	//We want to clear the scene every frame so things don't wdraw on top of each other
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	// uncomment this call to draw in wireframe polygons.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


	// draw our first triangle
	glUseProgram(m_shaderPrograms["Flat"]->m_programID);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	m_vaos["Triangle"]->Bind(); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
	m_vaos["Triangle"]->Draw();
	m_vaos["Triangle"]->Unbind();
}

void GLScene::DrawBuffers()
{
	//First Pass
	// bind to framebuffer and draw scene as we normally would to color texture 
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)

	// make sure we clear the framebuffer's content
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	DrawTriangle();

	// now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
							  // clear all relevant buffers
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessery actually, since we won't be able to see behind the quad anyways)
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(m_shaderPrograms["Screen"]->m_programID);
	m_vaos["Screen"]->Bind();
	glBindTexture(GL_TEXTURE_2D, m_fbTexture);	// use the color attachment texture as the texture of the quad plane
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	m_vaos["Screen"]->Draw();
	
}
//----------------------------------------------------------------
// @brief draw the scene
//----------------------------------------------------------------
void GLScene::Draw()
{
	DrawBuffers();
}

//----------------------------------------------------------------
// @brief called when the mouse is moved
//----------------------------------------------------------------
void GLScene::mouseMoveEvent(const SDL_MouseMotionEvent &_event)
{

}

//----------------------------------------------------------------
// @brief called when the mouse is pressed down
//----------------------------------------------------------------
void GLScene::mousePressEvent(const SDL_MouseButtonEvent &_event)
{

}

//----------------------------------------------------------------
// @brief called when the mouse is pressed up
//----------------------------------------------------------------
void GLScene::mouseReleaseEvent(const SDL_MouseButtonEvent &_event)
{

}

//----------------------------------------------------------------
// @brief called when the scroll wheel is moved
//----------------------------------------------------------------
void GLScene::mouseScrollEvent(const SDL_MouseWheelEvent &_event)
{

}

//----------------------------------------------------------------
// @brief called when the mouse is moved
//----------------------------------------------------------------
void GLScene::loadMatricesToShader()
{

}