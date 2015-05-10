in mediump vec2 frag_tex_position;
uniform sampler2D tex1;
uniform sampler2D tex2;

out mediump vec4 frag_color;

void main()
{
    mediump vec4 color1 = texture(tex1, frag_tex_position);
    mediump vec4 color2 = texture(tex2, frag_tex_position);
    color1.a = color2.a;
    frag_color = color2;
}
