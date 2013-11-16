#version 430 core

in VS_OUT
{
    vec2 TexCoord;
} fs_in;

uniform sampler2D sampler;

out vec4 FragColor;

void main()
{
    FragColor = texture(sampler, fs_in.TexCoord.xy);
}
