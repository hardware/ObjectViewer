#version 430 core

in VS_OUT
{
    vec2 texCoord;
} fs_in;

out vec4 FragColor;

uniform sampler2DArray texColor;

void main()
{
    FragColor = texture(texColor, vec3(fs_in.texCoord.xy, 1.0));
}
