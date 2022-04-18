#version 330

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;


// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform vec3 object_color;
uniform float Time;
uniform int head;
uniform int isHit[5];

// Output
out vec2 texcoord;
out vec4 normal; 


void main()
{
    // TODO(student): Pass v_texture_coord as output to fragment shader
    vec4 vecPosition = vec4(v_position, 1.0f);

    for (int i = 0; i < 5; i++)
        if ((isHit[i] == 1))
        {
            for (int i = 0; i < 114; i++)
            {
                if (gl_VertexID == i)
                {
                    vecPosition = vec4(v_position +  vec3(cos(-Time + i), cos(Time + i), 0) +  vec3(sin(-Time + i), sin(Time + i), 0), 1.0f), 1.0f;
                    //normal = vec4(v_normal + vec3(cos(i * Time/10000), Time * i/10000, sin(-Time * i/10000)), 1.0f);
                }
            }
        }

    if((head == 1))
        texcoord = v_texture_coord + vec2(0, -Time / 5);
    else 
        texcoord = v_texture_coord;

    gl_Position = Projection * View * Model * vecPosition;
}
