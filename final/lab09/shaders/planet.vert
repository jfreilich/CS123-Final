varying float intensity;
varying float height;
uniform sampler2D planet_texture;

void main()
{
    vec3 normal = normalize(gl_NormalMatrix * gl_Normal);
    vec3 light = normalize(gl_LightSource[0].position - (gl_ModelViewMatrix * gl_Vertex)).xyz;
    intensity = max(0.0, dot(normal, light)*2.0);


    gl_TexCoord[0] = gl_MultiTexCoord0;
    vec4 color=texture2D(planet_texture, gl_TexCoord[0].st);
    vec4 newPoint=gl_Vertex.xyzw;
    float colorIntensity=color.r*color.r+color.g*color.g+color.b*color.b;
    colorIntensity=colorIntensity/10.0;
    colorIntensity=colorIntensity+1.0;

    newPoint.x=colorIntensity*newPoint.x;
    newPoint.y=colorIntensity*newPoint.y;
    newPoint.z=colorIntensity*newPoint.z;
    gl_Position = gl_ModelViewProjectionMatrix*newPoint;
    gl_Position=ftransform();
    

    
    height = gl_Vertex.z;
}
