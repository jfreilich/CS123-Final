
varying vec3 vertex;		// The vector from the eye to the vertex
varying vec3 light;		// The normalized vector from the vertex to the light
varying vec3 eye;		// The normalized vector from the vertex to the eye
varying vec3 normal;		// The normal vector of the vertex, in eye space

uniform samplerCube envMap;	// The cube map containing the environment to reflect
uniform vec4 ambient;		// The ambient channel of the color to reflect
uniform vec4 diffuse;		// The diffuse channel of the color to reflect
uniform vec4 specular;		// The specular channel of the color to reflect
    
uniform float r0;		// The Fresnel reflectivity when the incident angle is 0
uniform float m;		// The roughness of the material

void main()
{
    vec3 e = normalize(eye);
    vec3 n = normalize(normal);
    vec3 i = normalize(vertex - eye);
    vec3 l = normalize(light);
    
    vec3 h = normalize(l - i);

    // TODO: fill this in
   
    
    float theta = dot(-i,n);
	
    float F = r0 + (1-r0) * pow(1-cos(theta), 5);
    float T = 1 - F;
    
    float alpha = acos(dot(n,h));	
    float D = (exp(-tan(alpha)*tan(alpha)/(m*m)))/(4.0*m*m*pow(cos(alpha),4.0));
    
    int G = min(min(1.0, 	
		(2 * (dot(h,n)) * (dot(e,n)))/(dot(e,h))), 
		(2 * (dot(h,n)) * (dot(l,n)))/(dot(e,h)));
    
    float specularCo = D*F*G/(dot(e,n));
	
    float diffuseCo = dot(n, l);

    vec3 r = reflect(-l, n);
    
    //intensity = intensity + pow(dot(e, r), specular);
    
    vec4 myColor = ambient + diffuse*max(0,diffuseCo) + specular*max(0,specularCo);
    
    //gl_FragColor = myColor;
       
    gl_FragColor = T*myColor + F*textureCube(envMap, -reflect(e,n)); //takes position
}
