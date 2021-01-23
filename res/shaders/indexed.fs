#version 140

in vec2 UV;
out vec3 color;

uniform sampler2D indexed_tex;
uniform sampler1D palette;

uniform vec2 indexed_res;
uniform float time;

void main(void) {

	float index = texture(indexed_tex, UV).r;
	color = texture(palette, index).rgb;

}