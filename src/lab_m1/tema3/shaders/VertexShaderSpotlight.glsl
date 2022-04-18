    #version 330

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform float Time;

// Output values to fragment shader
out vec3 world_position;
out vec3 world_normal;


void main()
{
    vec4 final_pos;
    vec3 v_pos = v_position;
    world_position = (Model * vec4(v_position, 1)).xyz;
    world_normal = normalize(mat3(Model) * normalize(v_normal));
    final_pos = vec4(v_position, 1.0);
    gl_Position = Projection * View * Model * final_pos;
}
