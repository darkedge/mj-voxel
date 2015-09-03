#version 450 core

layout( location = 0 ) out vec4 FragColor;

//in vec2 uv;
in vec4 color;
uniform sampler2D diffuseSampler;

void main( void ) {
	FragColor = vec4(color, 1.0f);
	//FragColor = texture( diffuseSampler, uv );
	//FragColor = vec4(gl_FragColor.a) * gl_FragColor + vec4(1.0 - gl_FragColor.a) * texture( diffuseSampler, uv );
}
