#version 330

// Input
in vec3 world_position;
in vec3 world_normal;

// Uniforms for light properties
uniform vec3 light_direction;
uniform vec3 light_position_roof;
uniform vec3 eye_position;
uniform vec3 disco_light_pos;
uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;
uniform float Time;
uniform sampler2D texture_1;
uniform int mode;
uniform int is_all;

// TODO(student): Declare any other uniforms
uniform vec3 object_color;

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    vec3 color;

    vec3 L = normalize(light_position_roof - world_position);

    vec3 L_disco;
    L_disco = normalize(disco_light_pos - world_position);

    vec3 V = normalize( eye_position - world_position);

    vec3 H = normalize(L + V);

    vec3 H_disco;
    H_disco = normalize(normalize(L_disco + V));

    vec3 R_disco;
    R_disco = reflect (-L_disco, world_normal);

    vec3 R = reflect (-L, world_normal);

    float ambient_light = 0.25 * material_kd;

    float diffuse_light = material_kd * 1 * max (dot(world_normal,L), 0);

    float specular_light = 0;

    if (diffuse_light > 0)
        specular_light = material_ks * pow(max(dot(V, R), 0), material_shininess);

    float ambient_light_disco = 0.1 * material_kd;

    float diffuse_light_disco;
    diffuse_light_disco = material_kd * 1 * max(dot(world_normal,L_disco), 0);

    float specular_light_disco;
    specular_light_disco = 0;

    if (diffuse_light_disco > 0)
        specular_light_disco = material_ks * pow(max(dot(V, R_disco), 0), material_shininess);

    float dist_disco;
    dist_disco =  distance(disco_light_pos, eye_position);

    float attenuation_factor_disco;
    attenuation_factor_disco = 1 / 1 / (1 + 0.01 * dist_disco + 0.05 * pow(dist_disco, 2));

    float dist = distance(light_position_roof, eye_position);
    float attenuation_factor = 1 / 1 / (1 + 0.01 * dist + 0.05 * pow(dist, 2));

    if (mode % 3 != 2 || is_all % 2 != 0)
    {
        color = 0.6 * object_color * (ambient_light + attenuation_factor * (8 * diffuse_light + specular_light));
        out_color = vec4(color,1);
    }

    if (mode % 3 == 2 || is_all % 2 != 0)
    {
        color = vec3(1);
        vec3 light_dir = world_position - disco_light_pos;
        vec2 texcooord;
        texcooord.x = (1.0 / (2 * 3.14159)) * atan (light_dir.x, light_dir.z);
        texcooord.y = (1.0 / 3.14159) * acos (light_dir.y / length (light_dir));

        if (Time > 0)
            texcooord = vec2(texcooord.x - Time / 10.f, texcooord.y);
	    
        color += 0.1 * (ambient_light_disco + attenuation_factor_disco * (50 * diffuse_light_disco + 10 * specular_light_disco));
        vec4 color1 = texture(texture_1, texcooord) / 4 * vec4(color, 0);
        out_color = color1;
    }
}
