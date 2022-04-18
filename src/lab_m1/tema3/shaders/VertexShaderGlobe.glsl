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
uniform int mode;
uniform int is_all;
uniform float Time;

// Output
out vec2 texcoord;
out int mode_globe;


void main()
{
	mode_globe = mode;
	texcoord = v_texture_coord;
    if ((Time > 0 && mode % 3 == 2) || is_all % 2 != 0)
		texcoord = vec2(texcoord.x - Time / 10.f, texcoord.y);
    gl_Position = Projection * View * Model * vec4(v_position, 1.0);
}
