#version 430 core

/*
PER-FRAGMENT LIGHTING (BLINN-PHONG SHADING IMPLEMENTATION)
*/

in vec3 position;
in vec4 color;
in vec2 texCoord;
in vec3 normal;

out VS_OUT
{
    vec3 N;
    vec3 L;
    vec3 V;

    vec2 texCoord;
    vec4 color;
} vs_out;

uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;

// Position of light
uniform vec3 lightPosition = vec3(100.0, 100.0, 100.0);

void main()
{
    // Calculate view-space coordinate
    vec4 P = modelViewMatrix * vec4(position, 1.0);

    // Calculate normal in view-space
    vs_out.N = mat3(modelViewMatrix) * normal;

    // Calculate view-space light vector
    vs_out.L = lightPosition - P.xyz;

    // Calculate view vector
    vs_out.V = -P.xyz;

    // Send texture coordinates and colors to the fragment shader
    vs_out.texCoord = texCoord;
    vs_out.color    = color;

    // Calculate the clip-space position of each vertex
    gl_Position = projectionMatrix * P;
}
