uniform sampler2D planet_texture;

varying float height;
varying float intensity;


/*vec4 itsownfunction(float regionMin, float regionMax, sampler2D regionTexture) {
    float regionRange = regionMax - regionMin;
    float regionWeight = (regionRange - abs(height - regionMax)) / regionRange;
    regionWeight = max(0.0,regionWeight);
    
    vec4 sample = texture2D(regionTexture, gl_TexCoord[0].st);
    
    return sample;// * regionWeight;
}

vec4 sampleTextures()
{    
    return itsownfunction(dirtMin, dirtMax, dirtTexture);/* +
    itsownfunction(grassMin, grassMax, grassTexture) +
    itsownfunction(rockMin, rockMax, rockTexture) +
    itsownfunction(snowMin, snowMax, snowTexture);   
}*/

void main()
{   
    gl_FragColor = texture2D(planet_texture, gl_TexCoord[0].st);// * intensity;//vec4(0.4,0.4,1,1);//
}
