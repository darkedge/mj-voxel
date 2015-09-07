#version 450 core

layout( location = 0 ) out vec4 FragColor;

in vec4 color;
in vec2 texcoord;

uniform sampler2D uMatAlbedo;

void main( void ) {
	FragColor = texture(uMatAlbedo, texcoord);
}
