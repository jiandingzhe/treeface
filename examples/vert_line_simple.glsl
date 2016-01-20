in highp vec2 position;
in mediump vec2 tangent_unorm;
in mediump float trip;
in mediump float side;

out mediump vec2 frag_tangent;
out mediump float frag_trip;
out mediump float frag_side;

void main()
{
    //vec2 fuck_me = position / 400.0;
    //gl_Position = vec4(fuck_me, 0.0, 1.0);
    gl_Position = matrix_model_view_project * vec4(position, 0.0, 1.0);
    frag_tangent = normalize(tangent_unorm);
    frag_trip = trip;
    frag_side = side;
}
