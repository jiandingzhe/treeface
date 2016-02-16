in highp vec2 position;

void main()
{
    gl_Position = matrix_model_view_project * vec4(position, 0.0, 1.0);
}
