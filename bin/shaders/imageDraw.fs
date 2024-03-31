#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
uniform sampler2D tex;

void main()
{  
    
        FragColor = vec4(texture(tex, TexCoords).rgb, 1.f);
       // FragColor = vec4(TexCoords.x,TexCoords.y,0.0,1.0);
             
   
}