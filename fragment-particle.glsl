
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
    //    vec3 pos = vec3(gl_TexCoord[0].x*aspectRatio*z, gl_TexCoord[0].y*z, -z);

    vec2 v = vec2(gl_TexCoord[0].x*aspectRatio, gl_TexCoord[0].y);

    float a = v.x;
    float b = v.y;
    float i = particle.x;
    float j = particle.y;
    float k = -particle.z;
    float r = particleSize;
    float raiz = -a*a*j*j - a*a*k*k + a*a*r*r + 2.0*a*b*i*j + 2.0*a*i*k - b*b*i*i - b*b*k*k + b*b*r*r + 2.0*b*j*k - i*i - j*j + r*r;
    raiz = sqrt(raiz);
    float fraccion = (a*i + b*j + k);
    float pznear = (-raiz + fraccion) / (a*a + b*b + 1.0);
    float pzfar = ( raiz + fraccion) / (a*a + b*b + 1.0);
    pznear = max(zNear, pznear);
    pzfar = max(zNear, pzfar);
    pzfar = min(pzfar, z);
    pznear = min(pznear, z);
    float light = abs(pznear - pzfar) / (r*2.0);
    light = light*light;
    vec3 lightCol = light * gl_Color.rgb;

    gl_FragColor = vec4(lightCol, 1.0);
}

