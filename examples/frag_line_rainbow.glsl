in mediump float trip;
in mediump float side;

out mediump vec4 frag_color;

mediump vec3 hsv2rgb(mediump vec3 hsv)
{
    mediump float hf = hsv.x * 6.0;
    int hi = int(floor(hf));
    mediump float s = hsv.y;
    mediump float v = hsv.z;

    mediump float f = hf - float(hi);
    mediump float p = v * (1.0 - s);
    mediump float q = v * (1.0 - f * s);
    mediump float t = v * (1.0 - (1.0 -f) * s);

    if      (hi == 0) { return vec3(v, t, p); }
    else if (hi == 1) { return vec3(q, v, p); }
    else if (hi == 2) { return vec3(p, v, t); }
    else if (hi == 3) { return vec3(p, q, v); }
    else if (hi == 4) { return vec3(t, p, v); }
    else if (hi == 5) { return vec3(v, p, q); }
}

void main()
{
    frag_color = vec4(hsv2rgb(vec3(trip, side, 1.0)), 1.0);
}
