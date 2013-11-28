#version 430 core

in vec3 position;
in vec2 texCoord;

out VS_OUT
{
    vec2 texCoord;
} vs_out;

uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;

void main()
{
    vs_out.texCoord = texCoord;
    gl_Position = projectionMatrix * modelViewMatrix * vec4(position, 1.0);
}
