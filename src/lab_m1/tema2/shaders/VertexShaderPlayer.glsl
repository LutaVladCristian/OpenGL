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
uniform int foot;
uniform int rightFoot;
uniform int hand;
uniform int rightHand;
uniform int animation;
uniform int pickUp;

// Output
out vec2 texcoord;


mat4 RotateOX(float radians)
{
    return mat4(
        1, 0, 0, 0,
        0, cos(radians), -sin(radians), 0,
        0, sin(radians), cos(radians), 0,
        0, 0, 0, 1);
}

mat4 RotateOZ(float radians)
{
    return mat4(
        cos(radians), -sin(radians), 0, 0,
        sin(radians), cos(radians), 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1);
}

void main()
{
    //animatie player
    vec4 vecPosition = vec4(v_position, 1.0f);

    if(animation == 1)
    {
        if ((hand == 1) && (rightHand == 1))
            vecPosition = RotateOX( 0.05 * cos(Time)) * RotateOZ( 0.05 * cos(2 * Time)) * vec4(v_position + vec3(0, -0.5 * cos(Time), -0.5 * cos(Time)), 1.0f);

        if ((hand == 1) && (rightHand == 0))
            vecPosition = RotateOX( 0.05 * cos(Time)) * RotateOZ( 0.05 * cos(2 * Time)) * vec4(v_position + vec3(0, 0.5 * cos(Time), 0.5 * cos(Time)), 1.0f);

        if ((foot == 1) && (rightFoot == 0))
            vecPosition = RotateOX( 0.05 * cos(Time)) * RotateOZ( 0.05 * cos(2 * Time)) * vec4(v_position + vec3(0, -0.5 * cos(Time), -0.5 * cos(Time)), 1.0f);

        if ((foot == 1) && (rightFoot == 1))
            vecPosition = RotateOX( 0.05 * cos(Time)) * RotateOZ( 0.05 * cos(2 * Time)) * vec4(v_position + vec3(0, 0.5 * cos(Time), 0.5 * cos(Time)), 1.0f);
    }

    if (pickUp == 1)
        texcoord = v_texture_coord + vec2(Time / 5, 0);
    else
        texcoord = v_texture_coord;

    gl_Position = Projection * View * Model * vecPosition;
}
