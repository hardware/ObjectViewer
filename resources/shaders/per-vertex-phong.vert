#version 430 core

/*
PER-VERTEX LIGHTING (GOURAUD SHADING IMPLEMENTATION)
*/

in vec3 position;
in vec2 texCoord;
in vec3 normal;

out VS_OUT
{
    vec3 light;
    vec2 texCoord;
} vs_out;

uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;

// Light properties
uniform vec3 lightPosition  = vec3(100.0, 100.0, 100.0);
uniform vec3 ambient        = vec3(0.1, 0.1, 0.1);
uniform vec3 diffuseAlbedo  = vec3(1.0, 1.0, 1.0);
uniform vec3 specularAlbedo = vec3(0.7);

uniform float specularPower = 64.0;

void main()
{
    // Calculate view-space coordinate
    vec4 P = modelViewMatrix * vec4(position, 1.0);

    // Calculate normal in view space
    vec3 N = mat3(modelViewMatrix) * normal;

    // Calculate view-space light vector
    vec3 L = lightPosition - P.xyz;

    // Calculate view vector
    vec3 V = -P.xyz;

    N = normalize(N);
    L = normalize(L);
    V = normalize(V);

    // Calculate the reflection of the light source in the surface
    vec3 R = reflect(-L, N);

    // Calculate the diffuse and specular highlight
    vec3 diffuse  = max(dot(N, L), 0.0) * diffuseAlbedo;
    vec3 specular = pow(max(dot(R, V), 0.0), specularPower) * specularAlbedo;

    // Send the final light color and texture coordinates to the fragment shader
    vs_out.light    = ambient + diffuse + specular;
    vs_out.texCoord = texCoord;

    // Calculate the clip-space position of each vertex
    gl_Position = projectionMatrix * P;
}
