#version 330

// Input
in vec3 world_position;
in vec3 world_normal;

// Uniforms for light properties
uniform vec3 light_direction;
uniform vec3 light_position;
uniform vec3 eye_position;
uniform int material_shininess;
uniform float material_ks;
uniform float material_kd;
uniform int is_spot;
uniform float cut_off;
uniform vec3 object_color;


// Output
layout(location = 0) out vec4 out_color;


void main()
{
	vec3 L = normalize(light_position - world_position);
	vec3 V = normalize(eye_position - world_position);
	vec3 H = normalize(L + V);
    vec3 N = normalize(world_normal);
	vec3 R = reflect(-L, N);

    float ambient_light = 0.25 * material_kd;
    float diffuse_light = material_kd * max(dot(N,L), 0);
    float specular_light = 0;

    if (diffuse_light > 0)
        specular_light = material_ks * pow(max(dot(V, R), 0), material_shininess);

    float dist, atenuationFactor; 
    float Light, light_att_factor, linear_att, spot_light, spot_light_limit;

    dist = distance(light_position, world_position);
    atenuationFactor = 1/1 / (1 + 0.1 * dist + 0.2 * pow(dist,2));
    
    if (is_spot == 0) 
    {
        Light = ambient_light + atenuationFactor * (diffuse_light + specular_light);
    }
    else 
    {
        spot_light = dot(-L, light_direction);
        spot_light_limit = cos(cut_off); 
        if (spot_light > cos(cut_off))
        {
            linear_att = (spot_light - spot_light_limit) / (1.0f - spot_light_limit);
            light_att_factor = pow(linear_att, 2);
		    Light = ambient_light + light_att_factor * atenuationFactor * (diffuse_light + specular_light);
        }
        else 
            Light = ambient_light;
	}


    out_color = vec4(object_color * Light, 1);

}
