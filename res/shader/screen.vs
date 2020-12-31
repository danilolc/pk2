#version 140

in vec2 position;
out vec2 UV;

void main(void) {

	gl_Position = vec4(2*position - 1.0, 0.0, 1.0);
	//gl_Position = vec4(position, 0.0, 1.0);
	UV = position;

}
