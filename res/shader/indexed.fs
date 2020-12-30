#version 150 core

in vec2 UV;
out uint color;

uniform usampler2D tex;

uint get_color(usampler2D image, uvec2 pos) {

	//ivec2 size = textureSize(image, 0);
	return texture(image, pos).r;

}

void main(void) {
	
	uvec2 pos = uvec2(gl_FragCoord.xy);
	//vec2 posf = gl_FragCoord.xy;
	uint c = pos.x;
	if (c >= 256u)
		c = 0u;
	color = c;

}
