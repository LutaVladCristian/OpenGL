#version 330

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;
layout(location = 3) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform vec2 mouse;
uniform int isSquare;

// Output
out vec2 texcoord;


void main()
{
    // TODO(student): Pass v_texture_coord as output to fragment shader
    vec4 vecPosition = vec4(v_position, 1.0f);
    texcoord = v_texture_coord;

    //Bonus 2
    /*if (isSquare == 1)
        vecPosition = vec4(v_position + vec3(mouse.x, 0.0f, mouse.y), 1.0f);*/

    gl_Position = Projection * View * Model * vecPosition;
}
