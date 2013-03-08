varying vec3 normal, vertex;

void main()
{
        normal = gl_NormalMatrix * gl_Normal;
        vertex = vec3(gl_ModelViewMatrix * gl_Vertex);

        gl_Position    = gl_ModelViewProjectionMatrix * gl_Vertex;
        gl_FrontColor  = gl_Color;
        gl_TexCoord[0] = gl_MultiTexCoord0;
}
