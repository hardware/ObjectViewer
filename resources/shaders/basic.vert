#version 430 core

in vec3 Position;
in vec2 TexCoord;
in vec3 Normal;

out VS_OUT
{
    vec2 TexCoord;
} vs_out;

uniform mat4 mvp;

void main()
{
    gl_Position = mvp * vec4(Position, 1.0);
    vs_out.TexCoord = TexCoord;
}
