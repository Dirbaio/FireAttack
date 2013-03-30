
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

    if(c2.a < 0.5)
    {
        //WTFLOL
    /*    vec2 v = vec2(gl_TexCoord[0].x*aspectRatio, gl_TexCoord[0].y);
        float a = v.x*v.x + v.y*v.y + 1.0;
        float b = -2.0*(v.x*particle.x + v.y*particle.y + particle.z);
        float c = particle.x*particle.x + particle.y*particle.y + particle.z*particle.z - particleSize*particleSize;
        float z1 = (-b + sqrt(b*b - 4.0*a*c))/2.0/a;
        float z2 = (-b - sqrt(b*b - 4.0*a*c))/2.0/a;
    */
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
        pzfar = max(pzfar, z);
        pznear = max(pznear, z);
        float light = abs(pznear - pzfar) / (r*2.0);
        light = light*light;
        vec3 lightCol = light * gl_Color.rgb;

    //    finalColor = vec3(int(floor(pos.x)) & 1, int(floor(pos.y)) & 1, int(floor(pos.z)) & 1);
        gl_FragColor = vec4(lightCol, 1.0);
    }
    else
        gl_FragColor = 0.0;

}

