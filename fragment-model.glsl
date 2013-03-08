varying vec3 normal, lightDir, eyeVec;

void main()
{

    /*
    vec4 final_color =
        (gl_FrontLightModelProduct.sceneColor * gl_FrontMaterial.ambient) +
        (gl_LightSource[0].ambient * gl_FrontMaterial.ambient) * 0.1;

    vec3 L = normalize(lightDir);

    float lambertTerm = dot(N,L);

    if(lambertTerm > 0.0)
    {
        final_color += gl_LightSource[0].diffuse *
                        gl_FrontMaterial.diffuse *
                        lambertTerm * 0.1;

        vec3 E = normalize(eyeVec);
        vec3 R = reflect(-L, N);
        float specular = pow( max(dot(R, E), 0.0),
                         gl_FrontMaterial.shininess );
        final_color += gl_LightSource[0].specular *
                       gl_FrontMaterial.specular *
                       specular;
    }

    vec3 E = normalize(eyeVec);
    vec3 R = reflect(-L, N);
    float lul = max(1.2-dot(N, E), 0.0);

    final_color += gl_LightSource[0].specular *
                   vec4(pow(lul,4)*0.7, pow(lul, 3), pow(lul,5), 1) * 0.4;
*/

    gl_FragData[0] = vec4(1.0, 1.0, 1.0, 1.0);
    vec3 N = normalize(normal);
    gl_FragData[1] = vec4((N*0.5)+0.5, 1.0);
}
