varying float intensity;

void main()
{
    gl_Position = ftransform();

    vec4 thing1 = gl_LightSource[0].position - 
		gl_ModelViewMatrix * gl_Vertex;
    
    vec3 thing2 = gl_NormalMatrix * gl_Normal;
    
    intensity = dot(normalize(thing1),normalize(thing2));
}
