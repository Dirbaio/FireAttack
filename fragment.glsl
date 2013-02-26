
uniform sampler2D tex;

void main()
{
/*    float a = 1.0 - sqrt(gl_TexCoord[0].x*gl_TexCoord[0].x + gl_TexCoord[0].y*gl_TexCoord[0].y);
    a = clamp(a, 0, 1);
    gl_FragColor = vec4(a, a, a, 1) * gl_Color;*/

    float c = texture2D(tex, gl_TexCoord[0]);

    c = c;

    gl_FragColor = vec4(c, c, c, 1);
}
