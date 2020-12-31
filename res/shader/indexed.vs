#version 140

in vec2 position;
out vec2 UV;

uniform vec2 buffer_resolution;

void main(void) {

	gl_Position = vec4(2*position - 1.0, 0.0, 1.0);
	UV = vec2(position.x, 1-position.y);

}
