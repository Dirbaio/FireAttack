
uniform vec3 cameraUp;
uniform vec3 cameraRight;
uniform vec3 cameraFront;
uniform vec3 cameraPos;

uniform sampler2D tex1;
uniform sampler2D tex2;
uniform sampler2D tex3;

uniform float w;
uniform float h;

void main()
{
    /*    float a = 1.0 - sqrt(gl_TexCoord[0].x*gl_TexCoord[0].x + gl_TexCoord[0].y*gl_TexCoord[0].y);
    a = clamp(a, 0, 1);
    gl_FragColor = vec4(a, a, a, 1) * gl_Color;*/

    vec2 coord = vec2(gl_FragCoord.x / w, gl_FragCoord.y / h);
    vec4 c1 = texture2D(tex1, coord);
    vec4 c2 = texture2D(tex2, coord);
    vec4 c3 = texture2D(tex3, coord);

    vec3 color = c1.rgb;
    vec3 normal = c2.xyz*2.0 - 1.0;


    vec3 lightDir = vec3(1, 1, 1);
    lightDir = normalize(lightDir);
    float light = dot(normal, lightDir);
    if(light < 0) light = 0;

    float zNear = 0.01;
    float zFar = 50;

    float x = coord.x*2.0-1.0;
    float y = coord.y*2.0-1.0;
    float z_b = c3.r;
    float z_n = 2.0 * z_b - 1.0;
    float z = 2.0 * zNear * zFar / (zFar + zNear - z_n * (zFar - zNear));
    vec3 ray = cameraUp*y+cameraRight*x+cameraFront;
    vec3 pos = cameraPos + ray*z;


    color = vec3(int(floor(pos.x)) & 1, int(floor(pos.y)) & 1, int(floor(pos.z)) & 1);
    vec3 finalColor = color;
    gl_FragColor += vec4(finalColor, 1);
}





