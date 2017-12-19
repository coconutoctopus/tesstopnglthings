#include "GLVAO.h"

GLVAO::GLVAO(GLenum _mode)
{
	//Create a VAO, assign it to our ID val
	glGenVertexArrays(1, &m_id);
	std::cout << "CREATING VAO" << m_id << std::endl;
	m_mode = _mode;
	m_data = new float[0];
}

GLVAO::~GLVAO()
{
	glDeleteVertexArrays(1, &m_id);
}

void GLVAO::Bind()
{
	//Binds this array
	//Any subsequent VBO/VAO/Attrib functions will be stored inside this bound VAO
	glBindVertexArray(m_id);
	m_bound = true;
}

void GLVAO::Unbind()
{
	//Sets the currently bound vao to 0 (null)
	glBindVertexArray(0);
	m_bound = false;
}

void GLVAO::Draw()
{
	if (m_allocated == false)
	{
		std::cerr << "Warning trying to draw an unallocated VOA\n";
	}
	if (m_bound == false)
	{
		std::cerr << "Warning trying to draw an unbound VOA\n";
	}
	//glDrawElements(m_mode, static_cast<GLsizei>(m_indicesCount), m_indexType, static_cast<GLvoid *>(nullptr));
	//std::cout << "glDrawArrays(" << m_mode << "," << 0 << "," << m_vertCount << ");" << std::endl;
	glDrawArrays(GL_TRIANGLES, 0, m_vertCount);
}


void GLVAO::setData(GLenum _target, float _data[], GLenum _usage, int _size)
{
	if (m_bound == false)
	{
		std::cerr << "trying to set VOA data when unbound\n";
	}

	m_data = _data;
	
	glGenBuffers(1, &m_vboID);

	// now we will bind an array buffer to the first one and load the data for the verts
	glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
	glBufferData(_target, _size, _data, _usage);

	m_allocated = true;

}

void GLVAO::removeVAO()
{
}

void GLVAO::setVertexAttributePointer(GLuint _id, GLint _size, GLenum _type, bool _normalise, int _stride, unsigned int _dataOffset)
{
	if (m_bound != true)
	{
		std::cerr << "Warning trying to set attribute on Unbound VOA\n";
	}

	glBindBuffer(GL_ARRAY_BUFFER, m_vboID);

	// set and enable the generic vertex attribute
	glVertexAttribPointer(
		_id,													//index
		_size,													//size : num components per vertex attrib-3 for vec3s
		_type,													//type
		_normalise,												//normalize
		_stride * sizeof(float),								//stride
		(void*)(_dataOffset * sizeof(float))					//pointer: the index of the first component of the attrib
	);

	glEnableVertexAttribArray(_id);
}

GLuint GLVAO::getBufferID(unsigned int _id)
{
	return m_id;
}

GLenum GLVAO::getMode() const
{
	return m_mode;
}

void GLVAO::setMode(const GLenum & _mode)
{
	m_mode = _mode;
}


