#version 430 core

/*
NORMAL MAPPING (AKA BUMP MAPPING)
+
PER-FRAGMENT LIGHTING (PHONG SHADING IMPLEMENTATION)
*/

in vec3 position;
in vec2 texCoord;
in vec3 normal;
in vec3 tangent;

out VS_OUT
{
    vec2 texCoord;
    vec3 eyeDirection;
    vec3 lightDirection;
} vs_out;

uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;

uniform vec3 lightPosition = vec3(0.0, 0.0, 100.0);

void main()
{
    vec4 P = modelViewMatrix * vec4(position, 1.0);
    vec3 V = P.xyz;
    vec3 L = lightPosition - P.xyz;
    vec3 N = normalize(mat3(modelViewMatrix) * normal);
    vec3 T = normalize(mat3(modelViewMatrix) * tangent);
    vec3 B = cross(N, T);

    vs_out.lightDirection = normalize(vec3(dot(L, T), dot(L, B), dot(L, N)));

    V = -P.xyz;

    vs_out.eyeDirection = normalize(vec3(dot(V, T), dot(V, B), dot(V, N)));
    vs_out.texCoord     = texCoord;

    gl_Position = projectionMatrix * P;
}
