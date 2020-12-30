#version 150 core

in vec2 UV;
out vec3 color;

uniform vec3 palette[256];
uniform usampler2D indexed_tex;

void main(void) {
	
	uint ll = texture(indexed_tex, UV).r;
	
	if (ll >= 256u)
		ll = 255u;
	
	color = palette[ll];

}
