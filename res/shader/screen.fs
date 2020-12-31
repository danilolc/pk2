#version 140

in vec2 UV;
out vec3 color;

uniform sampler2D screen_tex;

//uniform uvec2 buffer_resolution;
//uniform uvec2 screen_resolution;
//uniform vec4 screen_dst;
//uniform uint filter; //compile different shaders for each filter?

void main(void) {

	color = texture(screen_tex, UV).rgb;

}