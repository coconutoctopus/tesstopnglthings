#version 410 core

in vec3 vColour;
in vec2 vCoords;

///@brief output fragment colour
out vec4 fragColor;

uniform sampler2D Texture1;

void main()
{
	//Make orange
	fragColor = 1-texture(Texture1, vCoords);
}