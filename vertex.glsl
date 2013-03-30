varying vec3 normal, vertex;
uniform mat4 inverseLookAt;

void main()
{
        normal = gl_NormalMatrix * gl_Normal;
        vertex = vec4(inverseLookAt*gl_ModelViewMatrix * gl_Vertex).xyz;

        gl_Position    = gl_ModelViewProjectionMatrix * gl_Vertex;
        gl_FrontColor  = gl_Color;
        gl_TexCoord[0] = gl_MultiTexCoord0;
}
