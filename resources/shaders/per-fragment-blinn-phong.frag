#version 430 core

/*
PER-FRAGMENT LIGHTING (BLINN-PHONG SHADING IMPLEMENTATION)
*/

in VS_OUT
{
    vec3 N;
    vec3 L;
    vec3 V;

    vec2 texCoord;
} fs_in;

out vec4 FragColor;

// Texture sampler
uniform sampler2D sampler;

// Material properties
uniform vec3 diffuseAlbedo  = vec3(1.0, 1.0, 1.0);
uniform vec3 specularAlbedo = vec3(0.7);

uniform float specularPower = 64.0;

void main()
{
    // Normalize the incoming N, L and V vectors
    vec3 N = normalize(fs_in.N);
    vec3 L = normalize(fs_in.L);
    vec3 V = normalize(fs_in.V);

    // Calculate the half vector
    vec3 H = normalize(L + V);

    // Compute the diffuse and specular components for each fragment
    vec3 diffuse  = max(dot(N, L), 0.0) * diffuseAlbedo;
    vec3 specular = pow(max(dot(N, H), 0.0), specularPower) * specularAlbedo;

    // Write final color to the framebuffer
    FragColor = texture(sampler, fs_in.texCoord.xy) * vec4(diffuse + specular, 1.0);
}
