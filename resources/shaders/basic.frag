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
    vec3 direction; // Direction vector of the light source

    vec3 Ka; // Ambient light color
    vec3 Kd; // Diffuse light color
    vec3 Ks; // Specular light color

    float constantAttenuation;  // Constant light attenuation factor
    float linearAttenuation;    // Linear light attenuation factor
    float quadraticAttenuation; // Quadratic light attenuation factor

    float intensity; // Power scale of light source
    // float falloff;
    // float cosOuterAngle; // Angle of outer cone
    // float cosInnerAngle; // Angle of inner cone
} light;

const float cosOuterAngle = 0.76604;
const float cosInnerAngle = 0.86602;

uniform mat4 viewMatrix;
uniform vec3 globalAmbient = vec3(0.1, 0.1, 0.1);

// ###############################################################################

vec3 calculateLightComponents(vec3 L, vec3 N, vec3 V, float spotFactor, float attenuation)
{
    // Normalize L vector
    L = normalize(L);

    // Calculate R by reflecting -L around the plane defined by N
    vec3 R = reflect(-L, N);

    // Calculate Lambertian reflectance and specular factor
    float nDotL = max(dot(N, L), 0.0);
    float rDotV = max(dot(R, V), 0.0);

    // Compute the ambient / diffuse / specular / emissive components for each fragment
    vec3 emissive = material.Ke.xyz;
    vec3 ambient  = material.Ka.xyz * ( globalAmbient + (attenuation * spotFactor * light.Ka) );
    vec3 diffuse  = material.Kd.xyz * light.Kd * nDotL * attenuation * spotFactor;
    vec3 specular = vec3(0.0);

    if(nDotL > 0.0)
        specular = material.Ks.xyz * light.Ks * pow(rDotV, material.shininess) * attenuation * spotFactor;

    return ( ambient + diffuse + specular + emissive ) * light.intensity;
}

vec3 calculatePointLight(vec3 L, vec3 N, vec3 V, float spotFactor)
{
    // Calculate the length of model-space light vector
    float lightDistance = length(L);

    // Calculate light attenuation , see : RTR3 p219 (7.14)
    float attenuation = 1 / (light.constantAttenuation +
                             light.linearAttenuation * lightDistance +
                             light.quadraticAttenuation * lightDistance * lightDistance);

    // Calculate final light divided by attenuation
    return calculateLightComponents(L, N, V, spotFactor, attenuation);
}

vec3 calculateSpotLight(vec3 L, vec3 N, vec3 V)
{
    // Calculate the spot direction vector in view-space
    vec3 D = vec3(viewMatrix * vec4(light.direction, 1.0));

    // Calculate spot factor, see : RTR3 p221 (7.18)
    float spotFactor = clamp(
        pow((dot(normalize(-L), normalize(D)) - cosOuterAngle) / (cosInnerAngle - cosOuterAngle), 1.0),
        0.0,
        1.0
    );

    return calculatePointLight(L, N, V, spotFactor);
}

// ###############################################################################

void main()
{
    vec4 lightPosition = viewMatrix * vec4(light.position, 1.0);

    // Calculate view-space light unit vector (light direction)
    vec3 L = lightPosition.xyz - fs_in.P.xyz;

    // Normalize the incoming N and V vectors
    vec3 N = normalize(fs_in.N);
    vec3 V = normalize(fs_in.V);

    vec4 textureColor;

    if(material.hasTexture)
        textureColor = texture(texColor, fs_in.texCoord.xy);
    else
        textureColor = material.Kd;

    FragColor = textureColor * fs_in.color * vec4(calculateSpotLight(L, N, V), 1.0);
}
