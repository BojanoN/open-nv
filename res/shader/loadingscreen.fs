#version 330 core
out vec4 FragColor;
  
in vec2 TexCoord;

uniform sampler2D currentTexture;
uniform sampler2D nextTexture;

uniform float mixRatio = 0.0;

void main()
{
    FragColor = mix(texture(currentTexture, TexCoord), texture(nextTexture, TexCoord), mixRatio);
}