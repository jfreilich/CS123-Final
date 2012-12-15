varying float intensity;
varying float height;
uniform sampler2D planet_texture;
uniform vec4 average_color;
void main()
{
    vec3 normal = normalize(gl_NormalMatrix * gl_Normal);
    vec3 light = normalize(gl_LightSource[0].position - (gl_ModelViewMatrix * gl_Vertex)).xyz;
    intensity = max(0.0, dot(normal, light)*2.0);


    gl_TexCoord[0] = gl_MultiTexCoord0;
    vec4 color=texture2D(planet_texture, gl_TexCoord[0].st);
    vec4 difference=color-average_color;
    difference.w=0;
    float magnitude=sqrt(dot(difference,difference))/10.0;

    vec4 newPoint=gl_Vertex.xyzw;

  //  float colorIntensity=color.r*color.r+color.g*color.g+color.b*color.b+0.1;
  //  colorIntensity=sqrt(colorIntensity);
  //  colorIntensity=sqrt(colorIntensity);
 //   colorIntensity=colorIntensity/4.0;
   // colorIntensity=colorIntensity+1.0;
    float colorIntensity=1.0+magnitude;
    newPoint.x=colorIntensity*newPoint.x;
    newPoint.y=colorIntensity*newPoint.y;
    newPoint.z=colorIntensity*newPoint.z;
    gl_Position = gl_ModelViewProjectionMatrix*newPoint;
    //gl_Position=ftransform();
    

    
    height = gl_Vertex.z;
}
