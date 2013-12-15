#version 430 core

/*
PER-FRAGMENT LIGHTING (PHONG SHADING IMPLEMENTATION)
*/

in VS_OUT
{
    vec4 P;
    vec3 N;
    vec3 V;

    vec2 texCoord;
    vec4 color;
} fs_in;

out vec4 FragColor;

// Texture sampler
uniform sampler2D texColor;

// Material properties
uniform MaterialInfo
{
    vec4 Ka; // Ambient reflectivity
    vec4 Kd; // Diffuse reflectivity
    vec4 Ks; // Specular reflectivity
    vec4 Ke; // Emissive reflectivity

    float shininess; // Specular shininess exponent
    float shininessStrength; // Not used in Phong model
     bool hasTexture;
} material;

// Light properties
uniform struct LightInfo
{
    vec3 position;  // Position of light source

    vec3 Ka; // Ambient light color
    vec3 Kd; // Diffuse light color
    vec3 Ks; // Specular light color

    float intensity; // Power scale of light source
} light;

uniform vec3 globalAmbient = vec3(0.1, 0.1, 0.1);

void main()
{
    // Calculate view-space light unit vector (light direction)
    // and normalize the incoming N and V vectors
    vec3 L = normalize(light.position - fs_in.P.xyz);
    vec3 N = normalize(fs_in.N);
    vec3 V = normalize(fs_in.V);

    // Calculate R by reflecting -L around the plane defined by N
    vec3 R = reflect(-L, N);

    // Calculate Lambertian reflectance and specular factor
    float nDotL = max(dot(N, L), 0.0);
    float rDotV = max(dot(R, V), 0.0);

    // Compute the emissive / ambient / diffuse / specular components for each fragment
    vec3 emissive = material.Ke.xyz;
    vec3 ambient  = ( material.Ka.xyz * light.Ka ) + ( material.Ka.xyz * globalAmbient );
    vec3 diffuse  = material.Kd.xyz * light.Kd * nDotL;
    vec3 specular = vec3(0.0);

    if(nDotL > 0.0)
        specular = material.Ks.xyz * light.Ks * pow(rDotV, material.shininess);

    vec3 lightColor = ( emissive + ambient + diffuse + specular ) * light.intensity;
    vec4 textureColor;

    if(material.hasTexture)
        textureColor = texture(texColor, fs_in.texCoord.xy);
    else
        textureColor = material.Kd;

    FragColor = textureColor * fs_in.color * vec4(lightColor, 1.0);
}
