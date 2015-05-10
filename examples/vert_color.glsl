in highp vec4 position;
in mediump vec4 color;

out mediump vec4 vert_frag_color;

void main()
{
    gl_Position = matrix_model_view * position;
    vert_frag_color = color;
}
