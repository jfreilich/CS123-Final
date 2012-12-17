const int MAX_KERNEL_SIZE = 128;
uniform int arraySize;
uniform vec2 offsets[MAX_KERNEL_SIZE]; 
uniform float kernel[MAX_KERNEL_SIZE];
void main(void) { 
    arraySize = 3
    vec4 sum = vec4(0.0,0.0,0.0,0.0);
    for (int i=0;i<arraySize;i++){
        sum+=gl_TexCoord[0].st+i-arraySize;
    }
    gl_FragColor=gl_TexCoord[0].st;//sum;
}
