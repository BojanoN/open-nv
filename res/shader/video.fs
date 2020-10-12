#version 330 core
out vec4 FragColor;
  
in vec2 TexCoord;

uniform sampler2D videoFrame;

void main()
{
    FragColor = texture(videoFrame, TexCoord);
}