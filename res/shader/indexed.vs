#version 150 core

in vec2 vbo;
out vec2 UV;

uniform vec2 position;

void main(void) {

	gl_Position = vec4(vbo*0.5, 0.0, 1.0);
	UV = vbo;

}
