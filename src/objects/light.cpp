#include "light.h"
#include "../helpers/MathUtils.h"

using namespace Math;

const float INNER_ANGLE = 30.0f; // 30deg = 0.6981rad = cos(2PI/9) = 0.76604;
const float OUTER_ANGLE = 40.0f; // 40deg = 0.5235rad = cos(1PI/6) = 0.86602;

Light::Light(const string& name) :
    m_name(name),
    m_type(LightType::PointLight),
    m_position(Vector3D::ZERO),
    m_direction(Vector3D::UNIT_Z),
    m_ambientColor(Qt::black),
    m_diffuseColor(Qt::white),
    m_specularColor(Qt::black),
    m_constantAttenuation(1.0f),
    m_linearAttenuation(0.0f),
    m_quadraticAttenuation(0.0f),
    m_spotFalloff(1.0f),
    m_spotInnerAngle(cosf(static_cast<float>(M_PI)*(INNER_ANGLE/180.0f))),
    m_spotOuterAngle(cosf(static_cast<float>(M_PI)*(OUTER_ANGLE/180.0f))),
    m_intensity(1.0f)
{}

Light::~Light() {}

void Light::setType(LightType type)
{
    m_type = type;
}

Light::LightType Light::type() const
{
    return m_type;
}

void Light::setIntensity(float intensity)
{
    m_intensity = intensity;
}

void Light::setUniqueColor(const QColor& color)
{
    if(color.alpha())
        qWarning("The alpha channel of light colors should be equal to 0");

    m_ambientColor  = color;
    m_diffuseColor  = color;
    m_specularColor = color;
}

void Light::setUniqueColor(float r, float g, float b)
{
    m_ambientColor.setRgbF(r, g, b, 0.0);
    m_diffuseColor.setRgbF(r, g, b, 0.0);
    m_specularColor.setRgbF(r, g, b, 0.0);
}

void Light::setColors(const QColor& ambientColor,
                      const QColor& diffuseColor,
                      const QColor& specularColor)
{
    if(ambientColor.alpha() || diffuseColor.alpha() || specularColor.alpha())
        qWarning("The alpha channel of light colors should be equal to 0");

    m_ambientColor = ambientColor;
    m_diffuseColor = diffuseColor;
    m_specularColor = specularColor;
}

void Light::setAmbientColor(const QColor& color)
{
    if(color.alpha())
        qWarning("The alpha channel of ambient light color should be equal to 0");

    m_ambientColor = color;
}

void Light::setAmbientColor(float r, float g, float b)
{
    m_ambientColor.setRgbF(r, g, b, 0.0);
}

void Light::setDiffuseColor(const QColor& color)
{
    if(color.alpha())
        qWarning("The alpha channel of diffuse light color should be equal to 0");

    m_diffuseColor = color;
}

void Light::setDiffuseColor(float r, float g, float b)
{
    m_diffuseColor.setRgbF(r, g, b, 0.0);
}

void Light::setSpecularColor(const QColor& color)
{
    if(color.alpha())
        qWarning("The alpha channel of specular light color should be equal to 0");

    m_specularColor = color;
}

void Light::setSpecularColor(float r, float g, float b)
{
    m_specularColor.setRgbF(r, g, b, 0.0);
}

const QColor& Light::ambientColor() const
{
    return m_ambientColor;
}

const QColor& Light::diffuseColor() const
{
    return m_diffuseColor;
}

const QColor& Light::specularColor() const
{
    return m_specularColor;
}

void Light::setPosition(const QVector3D& position)
{
    if(m_type == DirectionalLight)
        qWarning("Directional lights not use position property");

    m_position = position;
}

void Light::setPosition(float x, float y, float z)
{
    if(m_type == DirectionalLight)
        qWarning("Directional lights not use position property");

    m_position.setX(x);
    m_position.setY(y);
    m_position.setZ(z);
}

const QVector3D& Light::position() const
{
    return m_position;
}

void Light::setDirection(const QVector3D& direction)
{
    if(m_type == PointLight)
        qWarning("Point lights not use direction property");

    m_direction = direction;
}

void Light::setDirection(float x, float y, float z)
{
    if(m_type == PointLight)
        qWarning("Point lights not use direction property");

    m_direction.setX(x);
    m_direction.setY(y);
    m_direction.setZ(z);
}

const QVector3D& Light::direction() const
{
    return m_direction;
}

void Light::setAttenuation(float constantFactor,
                           float linearFactor,
                           float quadraticFactor)
{
    if(m_type == DirectionalLight)
        qWarning("Directional lights not use attenuation property");

    m_constantAttenuation  = constantFactor;
    m_linearAttenuation    = linearFactor;
    m_quadraticAttenuation = quadraticFactor;
}

void Light::setConstantAttenuation(float constantFactor)
{
    if(m_type == DirectionalLight)
        qWarning("Directional lights not use constant attenuation property");

    m_constantAttenuation = constantFactor;
}

void Light::setLinearAttenuation(float linearFactor)
{
    if(m_type == DirectionalLight)
        qWarning("Directional lights not use linear attenuation property");

    m_linearAttenuation = linearFactor;
}

void Light::setQuadraticAttenuation(float quadraticFactor)
{
    if(m_type == DirectionalLight)
        qWarning("Directional lights not use quadratic attenuation property");

    m_quadraticAttenuation = quadraticFactor;
}

float Light::constantAttenuation() const
{
    return m_constantAttenuation;
}

float Light::linearAttenuation() const
{
    return m_linearAttenuation;
}

float Light::quadraticAttenuation() const
{
    return m_quadraticAttenuation;
}

void Light::setSpotFalloff(float falloff)
{
    if(m_type != SpotLight)
        qWarning("Only spotlights can set falloff property");

    m_spotFalloff = falloff;
}

void Light::setSpotInnerAngle(float innerAngle)
{
    if(m_type != SpotLight)
        qWarning("Only spotlights can set inner angle value");

    m_spotInnerAngle = cosf(static_cast<float>(M_PI)*(innerAngle/180.0f));
}

void Light::setSpotOuterAngle(float outerAngle)
{
    if(m_type != SpotLight)
        qWarning("Only spotlights can set outer angle value");

    m_spotOuterAngle = cosf(static_cast<float>(M_PI)*(outerAngle/180.0f));
}

float Light::spotFallOff() const
{
    return m_spotFalloff;
}

float Light::spotInnerAngle() const
{
    return m_spotInnerAngle;
}

float Light::spotOuterAngle() const
{
    return m_spotOuterAngle;
}

void Light::render(const QOpenGLShaderProgramPtr& shader, const QMatrix4x4& viewMatrix)
{
    shader->setUniformValue("light.intensity", m_intensity);

    shader->setUniformValue("light.Ka", m_ambientColor);
    shader->setUniformValue("light.Kd", m_diffuseColor);
    shader->setUniformValue("light.Ks", m_specularColor);

    switch(m_type)
    {
    case PointLight:
        shader->setUniformValue("light.position", viewMatrix * QVector4D(m_position, 1.0f));

        shader->setUniformValue("light.constantAttenuation",  m_constantAttenuation);
        shader->setUniformValue("light.linearAttenuation",    m_linearAttenuation);
        shader->setUniformValue("light.quadraticAttenuation", m_quadraticAttenuation);
        break;

    case SpotLight:
        shader->setUniformValue("light.position", viewMatrix * QVector4D(m_position, 1.0f));
        shader->setUniformValue("light.direction", viewMatrix * QVector4D(m_direction, 1.0f));

        shader->setUniformValue("light.constantAttenuation",  m_constantAttenuation);
        shader->setUniformValue("light.linearAttenuation",    m_linearAttenuation);
        shader->setUniformValue("light.quadraticAttenuation", m_quadraticAttenuation);

        shader->setUniformValue("light.spotFalloff",    m_spotFalloff);
        shader->setUniformValue("light.spotInnerAngle", m_spotInnerAngle);
        shader->setUniformValue("light.spotOuterAngle", m_spotOuterAngle);
        break;

    case DirectionalLight:
        shader->setUniformValue("light.direction", viewMatrix * QVector4D(m_direction, 1.0f));
        break;
    }
}
