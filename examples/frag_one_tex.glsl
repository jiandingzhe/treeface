in mediump vec2 frag_tex_position;
out mediump vec4 frag_color;
uniform sampler2D tex;

void main()
{
    frag_color = texture(tex, frag_tex_position);
}
