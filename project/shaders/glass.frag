
varying vec3 vertex;		// The position of the vertex, in eye space
varying vec3 light;		// The normalized vector from the vertex to the light
varying vec3 eye;		// The normalized vector from the vertex to the eye
varying vec3 normal;		// The normal vector of the vertex, in eye space

uniform float r0;		// The R0 value to use in Schlick's approximation
uniform float eta1D;		// The eta value to use initially
uniform vec3  eta;		// Contains one eta for each channel (use eta.r, eta.g, eta.b in your code)

uniform samplerCube envMap;

void main()
{
    vec3 n = normalize(normal);
    vec3 e = normalize(vertex);
    vec3 i = normalize(vertex - eye);
    
    // TODO: fill this in
    
    vec4 tr = gl_ModelViewMatrixInverse * vec4(refract(e, n, eta.r) , 0.0);
    vec4 tg = gl_ModelViewMatrixInverse * vec4(refract(e, n, eta.g) , 0.0);
    vec4 tb = gl_ModelViewMatrixInverse * vec4(refract(e, n, eta.b) , 0.0);
    
    //vec4 t = gl_ModelViewMatrixInverse * vec4(tr+tg+tb,1);
    
    float theta = dot(-i,n);
    float F = r0 + (1.0-r0) * pow(1.0-cos(theta), 5.0);
    float T = 1.0 - F;
    
    vec4 cR = vec4(textureCube(envMap, tr.r).rgb, 1.0);
    vec4 cG = vec4(textureCube(envMap, tg.g).rgb, 1.0);
    vec4 cB = vec4(textureCube(envMap, tb.b).rgb, 1.0);
    
    gl_FragColor = cR; //+ //T*textureCube(envMap, -reflect(e,n));
}
