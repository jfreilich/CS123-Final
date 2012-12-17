uniform sampler2D planet_texture;
uniform vec4 average_color;

void main()
{   
    gl_FragColor = texture2D(planet_texture, gl_TexCoord[0].st);
}
