varying vec3 normal;
varying vec3 vertex;
uniform vec3 color;
uniform float time;
uniform float time2;

void main()
{
    float y = 0.0-gl_TexCoord[0].y + time;
    float glow = exp(-5.0*fract(y))*0.1;

    float y2;

    y2 = -gl_TexCoord[0].y + time2*2.0;
    if(y2 > 0) glow += exp(-5.0*y2)*1.5;

    y2 = -gl_TexCoord[0].y + time2*3.0;
    if(y2 > 0) glow += exp(-5.0*y2)*1.5;

    //glow += max(0.0, 1.0-abs(vertex.y));
    if(vertex.y < 0.0) discard;
    gl_FragData[0] = vec4(color.xyz, glow);
    vec3 N = normalize(normal);
    gl_FragData[1] = vec4((N*0.5)+0.5, 1.0);
}
