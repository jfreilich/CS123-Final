varying float intensity;

void main()
{
    gl_Position=ftransform();
    vec3 normal=gl_NormalMatrix*gl_Normal;
    normalize(normal);
    vec3 light = gl_LightSource[0].position- gl_ModelViewMatrix*gl_Vertex;
    normalize(light);
    intensity=dot(normal,light);
}
