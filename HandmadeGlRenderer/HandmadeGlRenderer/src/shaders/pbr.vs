#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;
layout (location = 2) in vec3 aNormal;


out vec3 VertexPos;
out vec2 UV;
out vec3 Normal;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;


void main()
{
	UV = aUV;
	VertexPos = vec3(model * vec4(aPos, 1.0));
	Normal = mat3(model) * aNormal;

	gl_Position = projection * view * model * vec4(aPos, 1.0);
}

