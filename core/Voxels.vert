#version 450 core

layout (location = 0) in vec3 Position;
layout (location = 1) in vec4 Color;
layout (location = 2) in vec2 TexCoord;

uniform mat4 mvp_matrix;

out vec4 color;
out vec2 texcoord;

void main(void) {
	color = Color;
	gl_Position = mvp_matrix * vec4(Position, 1.0f);
	texcoord = TexCoord;
}
