in highp vec3 position;

void main()
{
    gl_Position = matrix_model_view_project * vec4(position,1);
}
