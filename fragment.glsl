
void main()
{
    float a = 1 - sqrt(gl_TexCoord[0].x*gl_TexCoord[0].x + gl_TexCoord[0].y*gl_TexCoord[0].y);
    gl_FragColor = vec4(a, a, a, 1) * gl_Color;
}
