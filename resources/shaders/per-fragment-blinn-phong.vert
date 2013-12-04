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
    vec3 V;
    vec4 P;

    vec2 texCoord;
    vec4 color;
} vs_out;

uniform mat4 modelMatrix;
uniform mat4 viewProjectionMatrix;

void main()
{
    // Calculate model-space coordinate
    vec4 P = modelMatrix * vec4(position, 1.0);

    // Calculate normal in model-space
    vs_out.N = mat3(modelMatrix) * normal;

    // Calculate model vector
    vs_out.V = -P.xyz;

    // Send the model-space coordinate to the fragment shader
    vs_out.P = P;

    // Send texture coordinates and colors to the fragment shader
    vs_out.texCoord = texCoord;
    vs_out.color    = color;

    // Calculate the clip-space position of each vertex
    gl_Position = viewProjectionMatrix * P;
}
