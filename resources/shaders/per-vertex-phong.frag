#version 430 core

in VS_OUT
{
    vec3 light;
    vec2 texCoord;
} fs_in;

out vec4 FragColor;

uniform sampler2D sampler;

void main()
{
    FragColor = texture(sampler, fs_in.texCoord.xy) * vec4(fs_in.light, 1.0);
}
