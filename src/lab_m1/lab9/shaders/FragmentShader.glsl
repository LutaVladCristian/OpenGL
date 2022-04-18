#version 330

// Input
in vec2 texcoord;

// Uniform properties
uniform sampler2D texture_1;
uniform sampler2D texture_2;
// TODO(student): Declare various other uniforms
uniform int mixed;
uniform float Time;
uniform int isEarth;
//uniform int isSquare;
//uniform vec2 mouse;

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    // TODO(student): Calculate the out_color using the texture2D() function.
    vec4 color1 = texture2D(texture_1,texcoord);
    vec4 color2 = texture2D(texture_2,texcoord); 

    if (mixed == 1)
        out_color = mix(color1, color2, 0.5f);
    else
        out_color = color1;
    if (out_color.a < 0.5f)
	    discard;

    if (isEarth == 1)
        out_color = texture2D(texture_1, texcoord - vec2(Time / 5, 0));
}
