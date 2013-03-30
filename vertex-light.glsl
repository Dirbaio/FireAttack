varying vec3 particle;
varying float particleSize;

void main()
{
        vec4 lol = gl_ModelViewMatrix * vec4(gl_MultiTexCoord0.xyz, 1);
        particle = lol.xyz;

        gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

        //Color de la particula
        gl_FrontColor = gl_Color;
        particleSize = gl_MultiTexCoord0.w;
        gl_TexCoord[0] = vec4(gl_Position.xy/gl_Position.z, 0, 0);
}
