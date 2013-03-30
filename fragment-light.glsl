
/*uniform vec3 cameraUp;
uniform vec3 cameraRight;
uniform vec3 cameraFront;
uniform vec3 cameraPos;

uniform float w;
uniform float h;
*/
uniform float aspectRatio;

uniform sampler2D tex1;
uniform sampler2D tex2;
uniform sampler2D tex3;

varying vec3 particle;
varying float particleSize;

void main()
{
    /*    float a = 1.0 - sqrt(gl_TexCoord[0].x*gl_TexCoord[0].x + gl_TexCoord[0].y*gl_TexCoord[0].y);
    a = clamp(a, 0, 1);
    gl_FragColor = vec4(a, a, a, 1) * gl_Color;*/

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
    vec3 pos = vec3(gl_TexCoord[0].x*aspectRatio*z, gl_TexCoord[0].y*z, -z);

    vec3 lightPos = particle;
    vec3 lightDir = lightPos - pos;
    float dist = length(lightDir);

    float light = dot(normal, lightDir)+0.5;
    light *= 1.0/(dist*dist);//(2.5 - sqrt(dist/sqrt(particleSize)));
    light = min(light, (particleSize/dist*30.0-1.0));
    light = max(0.0, light) * 0.5;
    vec3 lightCol = light * gl_Color.rgb;

    vec3 finalColor = color * lightCol;
//    finalColor = vec3(int(floor(pos.x)) & 1, int(floor(pos.y)) & 1, int(floor(pos.z)) & 1);
    gl_FragColor = vec4(finalColor, 1.0);
}

