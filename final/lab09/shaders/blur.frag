const int MAX_KERNEL_SIZE = 128;
uniform sampler2D tex;
uniform int arraySize;
uniform vec2 offsets[MAX_KERNEL_SIZE]; 
uniform float kernel[MAX_KERNEL_SIZE];
void main(void) { 
    vec4 sample;
    vec4 endColor = vec4(0,0,0,1);
    for (int i=0;i<arraySize*arraySize;i++) {

        sample = texture2D(tex, gl_TexCoord[0].st+offsets[i]);

        endColor += sample*kernel[i];
    }

    gl_FragColor = endColor;
}
