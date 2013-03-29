varying vec3 normal;
uniform vec3 color;
uniform float time;

void main()
{
    float y = 2.0-gl_TexCoord[0].y*2.0 + time;
    float glow = exp(-5.0*fract(y));
    gl_FragData[0] = vec4(color.xyz, glow);
    vec3 N = normalize(normal);
    gl_FragData[1] = vec4((N*0.5)+0.5, 1.0);
}
