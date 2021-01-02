#version 140

in vec2 uv_in;
out vec2 UV;

uniform vec2 buffer_resolution;

void main(void) {

	gl_Position = vec4(2*uv_in - 1.0, 0.0, 1.0);
	UV = vec2(uv_in.x, 1-uv_in.y);

}
