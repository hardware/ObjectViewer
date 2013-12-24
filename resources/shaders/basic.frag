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
    vec4 position;  // Position of light source in view-space
    vec4 direction; // Direction vector of the light source in view-space

    vec4 Ka; // Ambient light color
    vec4 Kd; // Diffuse light color
    vec4 Ks; // Specular light color

    float constantAttenuation;  // Constant light attenuation factor
    float linearAttenuation;    // Linear light attenuation factor
    float quadraticAttenuation; // Quadratic light attenuation factor

    float intensity; // Power scale of light source

    float spotFalloff; // The tightness of falloff between the inner and outer cones
    float spotInnerAngle; // Angle of outer cone (cosinus)
    float spotOuterAngle; // Angle of inner cone (cosinus)
} light;

uniform vec4 globalAmbient = vec4(0.1, 0.1, 0.1, 0.0);

// ###############################################################################

vec4 calculateLightComponents(vec3 L, vec3 N, vec3 V, float spotFactor, float attenuation)
{
    // Normalize L vector
    L = normalize(L);

    // Calculate R by reflecting -L around the plane defined by N
    vec3 R = reflect(-L, N);

    // Calculate Lambertian reflectance and specular factor
    float nDotL = max(dot(N, L), 0.0);
    float rDotV = max(dot(R, V), 0.0);

    // Compute the ambient / diffuse / specular / emissive components for each fragment
    vec4 emissive = material.Ke;
    vec4 ambient  = material.Ka * (globalAmbient + (light.Ka * attenuation * spotFactor));
    vec4 diffuse  = material.Kd * light.Kd * nDotL * attenuation * spotFactor;
    vec4 specular = vec4(0.0);

    if(nDotL > 0.0)
        specular = material.Ks * light.Ks * pow(rDotV, material.shininess) * attenuation * spotFactor;

    return ( ambient + diffuse + specular + emissive ) * light.intensity;
}

vec4 calculatePointLight(vec3 L, vec3 N, vec3 V, float spotFactor)
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

vec4 calculateSpotLight(vec3 L, vec3 N, vec3 V)
{
    // Calculate spot factor, see : RTR3 p221 (7.18)
    float spotFactor = clamp(
        pow((dot(normalize(-L), normalize(light.direction.xyz)) - light.spotOuterAngle) / (light.spotInnerAngle - light.spotOuterAngle), light.spotFalloff),
        0.0,
        1.0
    );

    return calculatePointLight(L, N, V, spotFactor);
}

// ###############################################################################

void main()
{
    // Calculate view-space light unit vector (light direction)
    vec3 L = light.position.xyz - fs_in.P.xyz;

    // Normalize the incoming N and V vectors
    vec3 N = normalize(fs_in.N);
    vec3 V = normalize(fs_in.V);

    vec4 textureColor;

    if(material.hasTexture)
        textureColor = texture(texColor, fs_in.texCoord.xy);
    else
        textureColor = material.Kd;

    FragColor = textureColor * fs_in.color * calculateSpotLight(L, N, V);
}
