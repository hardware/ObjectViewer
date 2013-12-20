#version 430 core

/*
PER-FRAGMENT LIGHTING (PHONG SHADING IMPLEMENTATION)
*/

in vec3 position; // Vertex postion
in vec4 color;    // Vertex color
in vec2 texCoord; // Texture coordinates
in vec3 normal;   // Vertex normal

out VS_OUT
{
    vec4 P; // Position vector transformed into eye (camera) space
    vec3 N; // Normal vector transformed into eye (camera) space
    vec3 V; // View vector (the negative of the view-space position)

    vec2 texCoord;
    vec4 color;
} vs_out;

uniform mat3 normalMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;

void main()
{
    // Calculate position vector in view-space coordinate
    vs_out.P = modelViewMatrix * vec4(position, 1.0);

    // Calculate normal vector in view-space coordinate
    // normalMatrix is the transpose of the inverse of the modelView matrix
    // normalMatrix = transpose(inverse(mat3(modelViewMatrix)));
    vs_out.N = normalMatrix * normal;

    // Calculate viewing vector from eye position to surface point
    vs_out.V = -vs_out.P.xyz;

    // Send texture coordinates and vertex color to the fragment shader
    vs_out.texCoord = texCoord;
    vs_out.color    = color;

    // Calculate the clip-space position of each vertex
    gl_Position = projectionMatrix * vs_out.P;
}
