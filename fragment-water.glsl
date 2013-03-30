varying vec3 normal;
uniform float time;

void main()
{
    float tx = gl_TexCoord[0].x+1;
    float ty = gl_TexCoord[0].y;
    float py = floor(ty / (1.154700538*3.0));
    float px = floor(tx / 2.0)+py;
    py *= 2.0;
    float x = fract(tx / 2.0);
    float y = fract(ty / (1.154700538*3.0));
    float x2 = abs(x-0.5)*2.0;
    float y2 = abs(y-0.5)*2.0;

    bool a = y2 < 1.0/3.0 + x2 / 3.0;
    bool top = y > 0.5;
    bool right = x > 0.5;

    if(a)
    {
        py += 1.0;
        if(right)
            px += 1.0;
    }
    else
        if(top)
        {
            py += 2.0;
            px += 1.0;
        }

    if(!a)
    {
        x2 = 1.0-x2;
        y2 = 1.0-y2;
    }

    float r = 0.0;

    if(y2*3.0 + x2 > 1.0)
        r = (y2*1.5 - x2*0.5)+0.5;
    else
        r = 1.0-x2;

    vec3 col = vec3(0.5, 0.7, 1.0);
    float glow = 0.0;
    float time2 = time*0.4;
    float d = tx*0.04+sin(ty*0.1+time2)*0.7;
    glow += exp(-5.0*fract(-r*0.5+time2+d));
    glow += exp(-5.0*fract(r*0.5+time2+d));

    float black = ty/40.0+1.0;
    black = clamp(black, 0.0, 1.0);
    glow *= black;

    gl_FragData[0] = vec4(col, glow);
    vec3 N = normalize(normal);
    gl_FragData[1] = vec4((N*0.5)+0.5, 0.0);
}
