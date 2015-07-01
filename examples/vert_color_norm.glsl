in highp vec4 position;
in mediump vec4 color;
in highp vec4 normal;

out highp vec4 color_v;
out highp vec3 normal_v;
out mediump vec3 eye_v;

void main()
{
    gl_Position = matrix_model_view_project * position;
    color_v = color;
    normal_v = (matrix_normal * normal).xyz;
    eye_v = (matrix_model_view * position).xyz;
}
