#version 450 core

layout( location = 0 ) out vec4 FragColor;

//in vec2 uv;
in vec4 color;
uniform sampler2D diffuseSampler;

void main( void ) {
	FragColor = color;
}
