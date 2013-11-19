#version 430 core

in vec3 position;
in vec2 texCoord;
in vec3 normal;

out VS_OUT
{
    vec2 texCoord;
} vs_out;

uniform mat4 mvp;

void main()
{
    gl_Position = mvp * vec4(position, 1.0);
    vs_out.texCoord = texCoord;
}
