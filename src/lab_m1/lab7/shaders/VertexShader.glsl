#version 330

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

// Uniforms for light properties
uniform vec3 light_position;
uniform vec3 eye_position;
uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;
uniform vec3 object_color;

// Output value to fragment shader
out vec3 color;


void main()
{
    vec3 world_Position = vec3 (Model * vec4(v_position, 1)).xyz;
    vec3 world_Normal = normalize (mat3(Model) * v_normal);
    vec3 L = normalize (light_position - world_Position);
    vec3 V = normalize ( eye_position - world_Position );
    vec3 H = normalize ( L + V );
    vec3 R = reflect (-L, world_Normal);

    float ambient_light = 0.25 * material_kd;

    float diffuse_light = material_kd * max(dot(world_Normal,L), 0);

    float specular_light = 0;

    if (diffuse_light > 0)
    {
        specular_light = material_ks * pow(max(dot(V, R), 0), material_shininess);
        //specular_light = material_ks * pow(max(dot(world_Normal, H), 0), material_shininess);
    }

    float dist = distance(light_position, world_Position);
    float atenuationFactor = 1/(0.5f * dist + 0.1f * pow(dist, 2));
    float light_intensity = ambient_light + atenuationFactor * (diffuse_light + specular_light);

    color = object_color * light_intensity;

    gl_Position = Projection * View * Model * vec4(v_position, 1.0);
}
