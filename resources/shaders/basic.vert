#version 430 core

in vec3 vertexPosition;
in vec2 TextureCoord;

out VS_OUT
{
    vec2 TextureCoord;
} vs_out;

uniform mat4 mvp;

void main()
{
    gl_Position = mvp * vec4(vertexPosition, 1.0);
    vs_out.TextureCoord = TextureCoord;
}
