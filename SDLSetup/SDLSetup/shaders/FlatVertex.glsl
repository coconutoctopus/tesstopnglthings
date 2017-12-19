#version 330 core

/// @brief[in] input vertex positions
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;	// the color variable has attribute position 1
layout(location = 2) in vec2 aTexCoords; // UVs

out vec3 vColour;
out vec2 vCoords;

void main()
{
	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
	vColour = aColor;
	vCoords = aTexCoords;
}