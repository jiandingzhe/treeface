in highp vec4 color_v;
in highp vec3 normal_v;
in mediump vec3 eye_v;

out highp vec4 frag_color;

void main()
{
    highp vec3 light_v = -global_light_direction.xyz;

    // diffuse
    highp float Kd = clamp( dot(light_v,normal_v), 0.0, 1.0);
    mediump vec4 diffuse = Kd * global_light_color;

    // specular
    highp vec3 half_v = normalize(light_v + eye_v);
    highp float Ks = pow( clamp(dot(half_v,normal_v), 0.0, 1.0), 2 ); // fixed shininess
    highp float f = 1.0;
    if (dot(normal_v,light_v) < 0.0)
    {
        f = 0.0;
    }
    highp vec4 specular = f * Ks * global_light_color;

    // final color
    frag_color = (global_light_ambient + diffuse) * color_v;
}
