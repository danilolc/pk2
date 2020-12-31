#version 140

in vec2 UV;
out vec3 color;

uniform vec3 palette[256];
uniform usampler2D indexed_tex;

void main(void) {

	uint idx = texture(indexed_tex, UV).r;
	color = palette[idx];

}