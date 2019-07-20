#version 330 core
layout (location = 0) in vec4 v;

out vec2 UV;

uniform mat4 projection;

void main()
{
   UV = v.zw;
   gl_Position = projection * vec4(v.xy, 0.0, 1.0);
};