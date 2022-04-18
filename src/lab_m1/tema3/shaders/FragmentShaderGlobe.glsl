#version 330

// Input
in vec2 texcoord;

// Uniform properties
uniform sampler2D texture_1;
uniform int mode;
uniform int is_all;

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    if (mode % 3 == 2 || is_all % 2 != 0)
    {    
        vec4 color1 = texture (texture_1, texcoord);
        out_color = color1;
        if (out_color.a < 0.5f)
            discard;
    }
    else
        out_color = vec4(0,0,0,1);
}
