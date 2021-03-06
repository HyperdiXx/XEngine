#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aTangent;
layout (location = 3) in vec3 aBitangent;
layout (location = 4) in vec2 aUV;

out vec2 uv;
out vec3 world_pos;
out vec3 normal;
out mat3 tbn;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main()
{
    uv = aUV;
    world_pos = vec3(model * vec4(aPos, 1.0));
    tbn = mat3(model) * mat3(aTangent, aBitangent, aNormal);
    gl_Position = projection * view * vec4(world_pos, 1.0);
}