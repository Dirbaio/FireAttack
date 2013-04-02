
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

uniform float time;
uniform mat4 inverseLookAt;
uniform vec3 cameraPos;
uniform vec3 cameraDir;
uniform vec3 cameraRight;
uniform vec3 cameraUp;

const float PI = 3.14159265358979323846264;

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
    vec4 pos = vec4(gl_TexCoord[0].x*aspectRatio*z, gl_TexCoord[0].y*z, -z, 1.0);
    pos = inverseLookAt*pos;

    vec3 dir = cameraDir;
    dir += cameraRight * gl_TexCoord[0].x * aspectRatio;
    dir += cameraUp * gl_TexCoord[0].y;

    //cameraPos + lambda*dir = (x, 0, y)
    vec3 pos2 = cameraPos;
    pos2 -= dir*(pos2.y/dir.y);

    float tx = pos2.x;
    float ty = pos2.z;
    bool sea = z_b < 0.99999 && pos.y > 0.0;
    if(sea)
    {
        tx = pos.x;
        ty = pos.z;
    }

    tx += 1.0;
    float py = floor(ty / (1.154700538*3.0));
    float px = floor(tx / 2.0)+py;
    py *= 2.0;
    float x = fract(tx / 2.0);
    float y = fract(ty / (1.154700538*3.0));
    float x2 = abs(x-0.5)*2.0;
    float y2 = abs(y-0.5)*2.0;

    bool a = y2 < 1.0/3.0 + x2 / 3.0;
    bool top = y > 0.5;
    bool right = x > 0.5;

    if(a)
    {
        py += 1.0;
        if(right)
            px += 1.0;
    }
    else
        if(top)

        {
            py += 2.0;
            px += 1.0;
        }

    if(!a)
    {
        x2 = 1.0-x2;
        y2 = 1.0-y2;
    }

    float r = 0.0;

    if(y2*3.0 + x2 > 1.0)
        r = (y2*1.5 - x2*0.5)+0.5;
    else
        r = 1.0-x2;

    float glow = 0.0;
    float time2 = time*0.4;
    float d = tx*0.04+sin(ty*0.1+time2)*0.7;
    if(sea)
    {
        glow = sin((r*0.5+time2+d)*2.0*PI)*0.5+0.5;
        glow *= 1.0-normal.y;
        glow *= exp(-pos.y);
    }
    else
    {
        glow += exp(-5.0*fract(-r*0.5+time2+d));
        glow += exp(-5.0*fract(r*0.5+time2+d));
        if(dir.y > 0.0) glow = 0.0;
    }

    float black = ty/40.0+1.0;
    black = clamp(black, 0.0, 1.0);

    vec3 col = vec3(0.0+0.1*glow, 0.1+glow*0.2, 0.4+glow*0.4);
    col *= black;
    gl_FragColor = vec4(col, 1.0);
}
