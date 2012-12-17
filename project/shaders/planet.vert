uniform sampler2D planet_texture;
uniform vec4 average_color;
void main()
{
    vec3 normal = normalize(gl_NormalMatrix * gl_Normal);
    gl_TexCoord[0] = gl_MultiTexCoord0;
    vec4 color=texture2D(planet_texture, gl_TexCoord[0].st);
    vec4 difference=color-average_color;
    difference.w=0;
    float magnitude=sqrt(dot(difference,difference))/10.0;
    vec4 newPoint=gl_Vertex.xyzw;
    float colorIntensity=1.0+magnitude;
    newPoint.x=colorIntensity*newPoint.x;
    newPoint.y=colorIntensity*newPoint.y;
    newPoint.z=colorIntensity*newPoint.z;
    gl_Position = gl_ModelViewProjectionMatrix*newPoint;
    //gl_Position=ftransform();
}
