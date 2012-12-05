
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

vec4 helper(float min, float maxx, sampler2D texture){
    float rR=maxx-min;
    float rW=(rR-abs(height-maxx))/rR;
    rW=max(0.0,rW);
    vec4 sample=texture2D(texture, gl_TexCoord[0].st);
    return sample*rW;
}
    vec4 sampleTextures()
{
    
    
    return helper(dirtMin, dirtMax,dirtTexture)+helper(grassMin, grassMax,grassTexture)+helper(rockMin, rockMax, rockTexture)+helper(snowMin, snowMax, snowTexture);
}

void main()
{   
    gl_FragColor = sampleTextures() * intensity;
}
