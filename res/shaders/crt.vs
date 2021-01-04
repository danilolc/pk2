#version 140

in vec2 vbo_in;
in vec2 uv_in;

out vec2 UV;

void main(void) {

	gl_Position = vec4(2*vbo_in - 1.0, 0.0, 1.0);
	UV = uv_in;

}
