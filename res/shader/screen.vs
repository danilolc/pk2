#version 140

in vec2 vbo;
out vec2 UV;

uniform vec4  screen_dst;

void main(void) {

	gl_Position = vec4(2*vbo-1.0, 0.0, 1.0);
	UV = vec2(vbo.x, 1-vbo.y);

}
