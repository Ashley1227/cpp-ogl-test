#version 330 core
out vec4 FragColor;

in vec2 texCoord;
in float depth;

uniform sampler2D ourTexture;

void main()
{
   FragColor = vec4(1-depth/128, 1-depth/128, 1-depth/128, 1);
//   FragColor = vec4(1,1,1,1);
//   FragColor = texture(ourTexture, texCoord);
}