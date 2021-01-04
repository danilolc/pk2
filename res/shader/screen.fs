#version 140

in vec2 UV;
out vec3 color;

uniform sampler2D screen_tex;

uniform vec2  buffer_res;
uniform vec2  screen_res;
uniform vec4  screen_dst;
uniform float time;

void main(void) {

	color = texture(screen_tex, UV).rgb;

}
