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

// Output
out vec2 texcoord;


void main()
{
    // TODO(student): Pass v_texture_coord as output to fragment shader
    vec4 vecPosition = vec4(v_position, 1.0f);

    if(head == 1)
        texcoord = v_texture_coord + vec2(0, -Time / 5);
    else
        texcoord = v_texture_coord;

    gl_Position = Projection * View * Model * vecPosition;
}
