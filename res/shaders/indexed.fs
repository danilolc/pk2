#version 140

in vec2 UV;
out vec3 color;

uniform usampler2D indexed_tex;
uniform vec3 palette[256];

uniform vec2 indexed_res;
uniform float time;

void main(void) {

	uint idx = texture(indexed_tex, UV).r;
	color = palette[idx];

}