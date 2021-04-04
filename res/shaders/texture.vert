#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 texCoord;
out float depth;

uniform mat4 mvp;
uniform float time;

void main()
{
//   gl_Position = mvp * vec4(aPos.x, aPos.y + sin(time*3)*4, aPos.z, 1.0f);
   gl_Position = mvp * vec4(aPos.x, aPos.y, aPos.z, 1.0f);
   texCoord = aTexCoord;

   depth = gl_Position.z;
}