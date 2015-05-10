in highp vec3 position;
in mediump vec2 tex_position;

out vec2 frag_tex_position;

void main()
{
    gl_Position = matrix_model_view * vec4(position, 1);
    frag_tex_position = tex_position;
}
