varying vec3 normal;

void main()
{
    float glow = 0;
    gl_FragData[0] = vec4(1.0, 1.0, 1.0, glow);
    vec3 N = normalize(normal);
    gl_FragData[1] = vec4((N*0.5)+0.5, 1.0);
}
