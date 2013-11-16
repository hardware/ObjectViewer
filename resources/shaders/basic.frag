#version 430 core

in VS_OUT
{
    vec2 TextureCoord;
} fs_in;

uniform sampler2D gSampler;

out vec4 color;

void main()
{
    color = texture(gSampler, fs_in.TextureCoord.xy);
}
