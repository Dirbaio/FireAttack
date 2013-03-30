varying vec3 normal;
varying vec3 vertex;
uniform vec3 color;
uniform float time;
uniform float time2;
uniform float time3;

void main()
{
    float y = 2.0-gl_TexCoord[0].y + time;
    float y2 = 2.0-gl_TexCoord[0].y + time2;
    float y3 = 2.0-gl_TexCoord[0].y + time3;
    float glow = exp(-5.0*fract(y))*0.5+exp(-7.0*fract(y2))+exp(-9.0*fract(y3));
//    glow += max(0.0, 1.0-abs(vertex.y));
    if(vertex.y < 0.0) discard;
    gl_FragData[0] = vec4(color.xyz, glow);
    vec3 N = normalize(normal);
    gl_FragData[1] = vec4((N*0.5)+0.5, 1.0);
}
