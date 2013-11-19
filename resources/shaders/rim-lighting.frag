#version 430 core

/*
PER-FRAGMENT LIGHTING (PHONG SHADING IMPLEMENTATION + RIM LIGHT EFFECT)
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

// Rim effect properties
uniform vec3  rimColor = vec3(0.93, 0.09, 0.14);
uniform float rimPower = 5.0;

vec3 calculateRim(vec3 N, vec3 V)
{
    // Calculate the rim factor
    float f = 1.0 - dot(N, V);

    // Constrain it to the range 0 to 1 using a smooth step function
    f = smoothstep(0.0, 1.0, f);

    // Raise it to the rim exponent
    f = pow(f, rimPower);

    // Finally, multiply it by the rim color
    return f * rimColor;
}

void main()
{
    // Normalize the incoming N, L and V vectors
    vec3 N = normalize(fs_in.N);
    vec3 L = normalize(fs_in.L);
    vec3 V = normalize(fs_in.V);

    // Calculate R locally
    vec3 R = reflect(-L, N);

    // Compute the diffuse and specular components + rim effect for each fragment
    vec3 diffuse  = max(dot(N, L), 0.0) * diffuseAlbedo;
    vec3 specular = pow(max(dot(R, V), 0.0), specularPower) * specularAlbedo;
    vec3 rim      = calculateRim(N, V);

    // Write final color to the framebuffer
    FragColor = texture(sampler, fs_in.texCoord.xy) * vec4(diffuse + specular + rim, 1.0);
}
