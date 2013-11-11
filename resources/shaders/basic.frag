#version 430 core

in VS_OUT
{
    vec2 TextureCoord;
} fs_in;

uniform sampler2D texture;

out vec4 color;

void main()
{
    color = texture2D(texture, fs_in.TextureCoord.xy);
}
