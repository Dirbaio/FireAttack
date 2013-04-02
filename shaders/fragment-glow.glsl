
uniform float aspectRatio;

uniform sampler2D tex1;
uniform sampler2D tex2;
uniform sampler2D tex3;


void main()
{
    vec2 coord = gl_TexCoord[0].xy*0.5 + 0.5;
    vec4 c1 = texture2D(tex1, coord); //Color texture
    vec4 c2 = texture2D(tex2, coord); //Normal texture
    vec4 c3 = texture2D(tex3, coord); //Depth texture

    vec3 color = c1.rgb;
    vec3 normal = c2.xyz*2.0 - 1.0;

    float zNear = 0.01;
    float zFar = 50.0;

    float z_b = c3.r;
    float z_n = 2.0 * z_b - 1.0;
    float z = 2.0 * zNear * zFar / (zFar + zNear - z_n * (zFar - zNear));
    vec3 pos = vec3(coord.x*aspectRatio*z, coord.y*z, -z);

    gl_FragColor = vec4(color * c1.a * 8, 1.0);

   /* vec3 res = vec3(0.0, 0.0, 0.0);
    vec2 pos = gl_TexCoord[0].xy*0.5 + 0.5;
    int rad = 2;
    float d = 0.002;
    for(int x = -rad; x <= rad; x++)
        for(int y = -rad; y <= rad; y++)
        {
            float st = exp(-(float(x*x)/(aspectRatio*aspectRatio)+float(y*y))*0.04);
            vec2 coord = pos + vec2(float(x)*d, float(y)*d)*st;
            vec4 c1 = texture2D(tex1, coord); //Color texture
            vec4 c2 = texture2D(tex2, coord); //Normal texture
            vec4 c3 = texture2D(tex3, coord); //Depth texture

            vec3 color = c1.rgb;
            vec3 normal = c2.xyz*2.0 - 1.0;

            float zNear = 0.01;
            float zFar = 50.0;

            float z_b = c3.r;
            float z_n = 2.0 * z_b - 1.0;
            float z = 2.0 * zNear * zFar / (zFar + zNear - z_n * (zFar - zNear));
            vec3 pos = vec3(coord.x*aspectRatio*z, coord.y*z, -z);

            res += color * c1.a/sqrt(z);

        }

    gl_FragColor = vec4(res*0.05, 1.0);*/
}

