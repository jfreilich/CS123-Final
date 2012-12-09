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
    vec4 newPoint=gl_vertex.xyzw;
    double colorIntensity=color.r*color.r+color.g*color.g+color.b*color.b;
    newPoint.x=newPoint.x+colorIntensity;
    newPoint.y=newPoint.y+colorIntensity;
    newPoint.z=newPoint.z+colorIntensity;
    gl_Position = gl_ModelViewProjectionMatrix*newPoint;
    

    
    height = gl_Vertex.z;
}
