#version 430 core

/*
PER-FRAGMENT LIGHTING (BLINN-PHONG SHADING IMPLEMENTATION)
*/

in VS_OUT
{
    vec3 N;
    vec3 V;
    vec4 P;

    vec2 texCoord;
    vec4 color;
} fs_in;

out vec4 FragColor;

// Texture sampler
uniform sampler2D texColor;

// Material properties
uniform struct MaterialInfo
{
    vec4 Ka; // Ambient reflectivity
    vec4 Kd; // Diffuse reflectivity
    vec4 Ks; // Specular reflectivity
    vec4 Ke; // Emissive reflectivity

    float shininess; // Specular shininess exponent
    float shininessStrength; // Not yet used
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
    float cutOff;    // Angle of spot light
} light;

// Rim effect properties
uniform vec3  rimColor = vec3(0.93, 0.09, 0.14);
uniform float rimPower = 5.0;

// ###############################################################################

// Calcule rim lighting
vec3 calculateRim(vec3 N, vec3 V)
{
    float f = 1.0 - dot(N, V);

    f = smoothstep(0.0, 1.0, f);
    f = pow(f, rimPower);

    return f * rimColor;
}

// ###############################################################################

// Light mode subroutine
subroutine vec3 lightColor(vec3 N, vec3 L, vec3 V);

layout (index = 1)
subroutine(lightColor)
vec3 PerFragmentBlinnPhong(vec3 N, vec3 L, vec3 V)
{
    // Calculate the half vector
    vec3 H = normalize(L + V);

    return pow(max(dot(N, H), 0.0), material.shininess) * material.Ks.xyz;
}

layout (index = 2)
subroutine(lightColor)
vec3 PerFragmentPhong(vec3 N, vec3 L, vec3 V)
{
    // Calculate R locally
    vec3 R = reflect(-L, N);

    return pow(max(dot(R, V), 0.0), material.shininess) * material.Ks.xyz;
}

layout (index = 3)
subroutine(lightColor)
vec3 RimLighting(vec3 N, vec3 L, vec3 V)
{
    vec3 R = reflect(-L, N);

    vec3 specular = pow(max(dot(R, V), 0.0), material.shininess) * material.Ks.xyz;
    vec3 rim      = calculateRim(N, V);

    return specular + rim;
}

subroutine uniform lightColor calculateSpecularColor;

// ###############################################################################

vec3 calculateLightComponents(vec3 L, vec3 N, vec3 V)
{
    // Compute the emissive / ambient / diffuse / specular components for each fragment
    vec3 emissive = material.Ke.xyz;
    vec3 ambient  = material.Ka.xyz * light.Ka;
    vec3 diffuse  = max(dot(N, L), 0.0) * material.Kd.xyz * light.Kd;
    vec3 specular = calculateSpecularColor(N, L, V) * light.Ks;

    // Sum all components and return final color
    return ( emissive + ambient + diffuse + specular ) * light.intensity;
}

vec3 calculatePointLight(vec3 L, vec3 N, vec3 V)
{
    // Calculate the length of model-space light vector
    float lightDistance = length(L);

    // Calculate light attenuation
    float attenuation = light.constantAttenuation +
                        light.linearAttenuation * lightDistance +
                        light.quadraticAttenuation * lightDistance * lightDistance;

    L = normalize(L);

    // Calculate final light divided by attenuation
    vec3 color = calculateLightComponents(L, N, V) / attenuation;

    return color;
}

vec3 calculateSpotLight(vec3 L, vec3 N, vec3 V)
{
    float spotFactor = dot(normalize(-L), normalize(light.direction));

    if(acos(spotFactor) < radians(light.cutOff))
        return calculatePointLight(L, N, V);
}

// ###############################################################################

void main()
{
    // Calculate model-space light vector (light direction)
    vec3 L = light.position - fs_in.P.xyz;

    // Normalize the incoming N and V vectors
    vec3 N = normalize(fs_in.N);
    vec3 V = normalize(fs_in.V);

    // Write final color to the framebuffer
    // FINALCOLOR = TEXTURECOLOR * VERTEXCOLOR * LIGHTCOLOR
    FragColor = texture(texColor, fs_in.texCoord.xy) * fs_in.color * vec4(calculateSpotLight(L, N, V), 1.0);
}
