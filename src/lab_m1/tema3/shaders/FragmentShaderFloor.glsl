#version 330

// Input
in vec3 world_position;
in vec3 world_normal;

// Uniforms for light properties
uniform vec3 light_direction_spot;
uniform vec3 color_spot[4];
uniform vec3 spot_position[4];
uniform vec3 eye_position;
uniform vec3 disco_light_pos;
uniform float Time;
uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;
uniform int mode;
uniform int is_all; 
uniform sampler2D texture_1;
uniform vec3 object_color;
uniform float cuttOffAngle;

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    vec3 color;
    vec3 L_disco;
    L_disco = normalize(disco_light_pos - world_position);

    vec3 V = normalize(eye_position - world_position);

    vec3 H_disco;
    H_disco = normalize(normalize(L_disco + V));

    vec3 R_disco;
    R_disco = reflect (-L_disco, world_normal);

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

    if (mode % 3 == 1 || is_all % 2 != 0)
    {
        color = vec3(object_color * 0.4);

        vec3 L[4];
        for (int i = 0; i < 4; i++)
            L[i] = normalize(spot_position[i] - world_position);

        vec3 V = normalize( eye_position - world_position);

        vec3 H[4];
        for (int i = 0; i < 4; i++)
            H[i] = normalize(L[i] + V);

        vec3 R[4];
        for (int i = 0; i < 4; i++)
            R[i] = reflect (-L[i], world_normal);

        float ambient_light = 0.25 * material_kd;

        float diffuse_light[4];
        for (int i = 0; i < 4; i++)
            diffuse_light[i] = material_kd * 1 * max (dot(world_normal,L[i]), 0);

        float specular_light[4];
        for (int i = 0; i < 4; i++)
            specular_light[i] = 0;

        for (int i = 0; i < 4; i++)
            if (diffuse_light[i] > 0)
                specular_light[i] = material_ks * pow(max(dot(V, R[i]), 0), material_shininess);

        float cut_off = radians(cuttOffAngle);

        float spot_light[4];

        for (int i = 0; i < 4; i++)
            spot_light[i] = dot(-L[i], light_direction_spot);

        float spot_light_limit = cos(cut_off);

        float linear_att[4];

        float light_att_factor[4];

        for (int i = 0; i < 4; i++)
        {
            if (spot_light[i] > cos(cut_off))
            {
                linear_att[i] = (spot_light[i] - spot_light_limit) / (1.0f - spot_light_limit);
                light_att_factor[i] = pow(linear_att[i], 2);
                color += color_spot[i]* (ambient_light + light_att_factor[i] * (diffuse_light[i] + specular_light[i]));
            }
            else
                color += 0.5 * ambient_light;
        }
        out_color = vec4(color,1);
    }

    if (mode % 3 == 0 || is_all % 2 != 0)
    {
        color = object_color * 1;
        out_color = vec4(color, 1);
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

        color += 0.1 * (ambient_light_disco + attenuation_factor_disco* (50 * diffuse_light_disco + 10 * specular_light_disco));
        vec4 color1 = texture(texture_1, texcooord) / 4 * vec4(color,0);
        out_color = color1 + vec4(object_color/5,0);   
    }
}
