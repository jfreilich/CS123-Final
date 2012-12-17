
uniform float dirtMin, dirtMax;
uniform float grassMin, grassMax;
uniform float rockMin, rockMax;
uniform float snowMin, snowMax;
    
uniform sampler2D dirtTexture;
uniform sampler2D grassTexture;
uniform sampler2D rockTexture;
uniform sampler2D snowTexture;

varying float height;
varying float intensity;


vec4 itsownfunction(float regionMin, float regionMax, sampler2D regionTexture) {
    float regionRange = regionMax - regionMin;
    float regionWeight = (regionRange - abs(height - regionMax)) / regionRange;
    regionWeight = max(0.0,regionWeight);
    
    vec4 sample = texture2D(regionTexture, gl_TexCoord[0].st);
    
    return sample * regionWeight;
}

vec4 sampleTextures()
{    
    return itsownfunction(dirtMin, dirtMax, dirtTexture) +
    itsownfunction(grassMin, grassMax, grassTexture) +
    itsownfunction(rockMin, rockMax, rockTexture) +
    itsownfunction(snowMin, snowMax, snowTexture);   
}

void main()
{   
    gl_FragColor = sampleTextures() * intensity;
}
