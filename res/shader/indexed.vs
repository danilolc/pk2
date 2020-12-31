#version 140

in vec2 vbo;
out vec2 UV;

void main(void) {

	gl_Position = vec4(2*vbo - 1.0, 0.0, 1.0);
	UV = vbo;

}
