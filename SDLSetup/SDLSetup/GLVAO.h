#pragma once
/*
Copyright (C) 2016 Jon Macey
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
//----------------------------------------------------------------------------------------------------------------------
/// @class AbstractVAO "include/AbstractVAO.h"
/// @brief base class for all VAO from the VAOFactory this defines the
/// base class type with simple draw / bind behaviour, the user must allocate the data and
/// do any more complex setup. See SimpleVAO for examples as well as the other ngl examples
/// @author Jonathan Macey
/// @version 1.0
/// @date 6/4/16
//----------------------------------------------------------------------------------------------------------------------


#include "GLUtil.h"

class GLVAO
{
public:
	GLVAO(GLenum _mode);
	~GLVAO();

	//----------------------------------------------------------------------------------------------------------------------
	/// @brief bind the VAO so it can be used.
	//----------------------------------------------------------------------------------------------------------------------
	void Bind();
	//----------------------------------------------------------------------------------------------------------------------
	/// @brief unbind the VAO by binding default 0
	//----------------------------------------------------------------------------------------------------------------------
	void Unbind();
	//----------------------------------------------------------------------------------------------------------------------
	/// @brief this is the draw method for the VAO the user must implement this per VAO data, usually this
	/// will be a call to glDrawArrays type function, VAO must be bound before calling this
	//----------------------------------------------------------------------------------------------------------------------
	void Draw();
	//----------------------------------------------------------------------------------------------------------------------
	/// @brief this method is used to set the data in the VAO, we have a base data type of
	/// VertexData above, but the user can extend this to create custom data types
	//----------------------------------------------------------------------------------------------------------------------
	void setData(GLenum _target, float _data[], GLenum _usage, int _size);
	//----------------------------------------------------------------------------------------------------------------------
	/// @brief this will clean up the VAO and associated data, it is the users responsibility to do this
	/// usually this will be a call to the various delete buffers gl routines
	//----------------------------------------------------------------------------------------------------------------------
	void removeVAO();
	//----------------------------------------------------------------------------------------------------------------------
	/// @brief set the generic vertex attribute pointer data usually this method will do however the user
	/// may occasionally need to override this method.
	/// @param _size the size of the raw data passed (not counting sizeof(GL_FLOAT))
	/// @param _type the data type of the Pointer (eg GL_FLOAT)
	/// @param _stride the step between data values (need *sizeof _type) e.g. 10*sizeof(GLfloat) 0 means tightly packed / contiguous
	/// @param _dataOffset Specifies a pointer to the first component of the first generic vertex attribute in the array.
	/// The initial value is 0. and is calculated as static_cast<Real *>(NULL)  + _dataOffset
	/// @param _normalise specifies whether fixed-point data values should be normalized (GL_TRUE) or converted directly as
	/// fixed-point values (GL_FALSE) when they are accessed.
	//----------------------------------------------------------------------------------------------------------------------
	void setVertexAttributePointer(GLuint _id, GLint _size, GLenum _type, bool _normalise, GLsizei _stride, unsigned int _dataOffset);
	//----------------------------------------------------------------------------------------------------------------------
	/// @brief the number of indices to draw in the array. It may be that the draw routine can overide this at another time.
	/// @param _s the number of indices to draw (from 0)
	//----------------------------------------------------------------------------------------------------------------------
	void setNumVerts(int _s) { m_vertCount = _s; }
	int numVerts() const { return m_vertCount; }
	//----------------------------------------------------------------------------------------------------------------------
	/// @brief return the id of the buffer, if there is only 1 buffer just return this
	/// if we have the more than one buffer the sub class manages the id's
	/// @param _buffer index (default to 0 for single buffer VAO's)
	//----------------------------------------------------------------------------------------------------------------------
	GLuint getBufferID(unsigned int _id = 0);
	//----------------------------------------------------------------------------------------------------------------------
	/// @brief get the draw mode
	//----------------------------------------------------------------------------------------------------------------------
	GLenum getMode() const;
	//----------------------------------------------------------------------------------------------------------------------
	/// @brief set the draw mode
	/// @param _mode (GL_TRIANGLES etc)
	//----------------------------------------------------------------------------------------------------------------------
	void setMode(const GLenum &_mode);

	//----------------------------------------------------------------------------------------------------------------------
	/// @brief grab a raw pointer to the data
	/// @param _index the index to the buffer (0) default for simple
	/// @param _mode the access more
	//----------------------------------------------------------------------------------------------------------------------
	int* mapBuffer(unsigned int _index = 0, GLenum _accessMode = GL_READ_WRITE);
	//----------------------------------------------------------------------------------------------------------------------
	/// @brief unmap the mapped buffer
	//----------------------------------------------------------------------------------------------------------------------
	void unmapBuffer() const;


protected:
	//----------------------------------------------------------------------------------------------------------------------
	/// @brief the draw mode
	//----------------------------------------------------------------------------------------------------------------------
	GLenum m_mode = GL_TRIANGLES;
	//----------------------------------------------------------------------------------------------------------------------
	/// @brief the id of the VAO allocated from OpenGL
	//----------------------------------------------------------------------------------------------------------------------
	GLuint m_id = 0;
	//----------------------------------------------------------------------------------------------------------------------
	/// @brief debug flag to indicate if the vao is bound.
	//----------------------------------------------------------------------------------------------------------------------
	bool m_bound = false;
	//----------------------------------------------------------------------------------------------------------------------
	/// @brief debug flag to indicate if data has been set for the VAO
	//----------------------------------------------------------------------------------------------------------------------
	bool m_allocated = false;
	//----------------------------------------------------------------------------------------------------------------------
	/// @brief the number of indices stored in the VAO.
	//----------------------------------------------------------------------------------------------------------------------
	int m_vertCount = 0;
	//----------------------------------------------------------------------------------------------------------------------
	/// @brief the data currently stored in the VBO
	//----------------------------------------------------------------------------------------------------------------------
	float* m_data;
	//----------------------------------------------------------------------------------------------------------------------
	/// @brief the VBOs
	//----------------------------------------------------------------------------------------------------------------------
	GLuint m_vboID = 0;

};

