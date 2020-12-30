#version 140

in vec2 UV;
out vec3 color;

uniform vec3 palette[256];
uniform usampler2D indexed_tex;

//uniform uvec2 buffer_resolution;
//uniform uvec2 screen_resolution;
//uniform vec4 screen_dst;
//uniform uint filter; //compile different shaders for each filter?

vec3 getTexture(vec2 uv){

	uint ll = texture(indexed_tex, uv).r;
	return palette[ll];
}

// Gaussian kernel
// 1 2 1
// 2 4 2
// 1 2 1	
const float kernel[9] = float[9]( 1.0/16.0, 2.0/16.0, 1.0/16.0,
				2.0/16.0, 4.0/16.0, 2.0/16.0,
				1.0/16.0, 2.0/16.0, 1.0/16.0 );

uniform sampler2D colorMap;

const float width = 1920;
const float height = 1080;

const float step_w = 1.0/width;
const float step_h = 1.0/height;

const vec2 offset[9] = vec2[9]( vec2(-step_w, -step_h), vec2(0.0, -step_h), vec2(step_w, -step_h), 
				vec2(-step_w, 0.0), vec2(0.0, 0.0), vec2(step_w, 0.0), 
				vec2(-step_w, step_h), vec2(0.0, step_h), vec2(step_w, step_h) );
void main(void)
{
	int i = 0;
	vec3 sum = vec3(0.0);
   
	for( i=0; i<9; i++ )
	{
		vec3 tmp = getTexture(UV.st + offset[i]);
		sum += tmp * kernel[i];
	}
	color = sum;

	//color = getTexture(UV);
}