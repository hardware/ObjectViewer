#version 430 core

/*
NORMAL MAPPING (AKA BUMP MAPPING)
+
PER-FRAGMENT LIGHTING (BLINN-PHONG SHADING IMPLEMENTATION)
*/

in VS_OUT
{
    vec2 texCoord;
    vec3 eyeDirection;
    vec3 lightDirection;
} fs_in;

out vec4 FragColor;

// Texture samplers
uniform sampler2D texColor;
uniform sampler2D texNormal;

uniform float specularPower = 64.0;

void main()
{
    vec3 V = normalize(fs_in.eyeDirection);
    vec3 L = normalize(fs_in.lightDirection);
    vec3 N = normalize(texture(texNormal, fs_in.texCoord).rgb * 2.0 - vec3(1.0));
    // N = vec3(0.0, 0.0, 1.0);
    vec3 R = reflect(-L, N);

    vec3 diffuseAlbedo = texture(texColor, fs_in.texCoord).rgb;
    vec3 diffuse = max(dot(N, L), 0.0) * diffuseAlbedo;
    vec3 specularAlbedo = vec3(0.7);
    // vec3 specular = max(pow(dot(R, V), 5.0), 0.0) * specularAlbedo;
    vec3 specular = pow(max(dot(R, V), 0.0), specularPower) * specularAlbedo;

    FragColor = vec4(diffuse + specular, 1.0);
}
