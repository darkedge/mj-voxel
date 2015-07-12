#version 450 core

layout (location = 0) in vec3 Position;
//layout (location = 2) in vec2 TexCoord;

uniform mat4 mvp_matrix;

//out vec2 uv;

void main(void) {
	gl_Position = mvp_matrix * vec4(Position, 1.0f);
	//uv = TexCoord;
}
