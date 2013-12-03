#include "abstractlight.h"

AbstractLight::AbstractLight()
    : m_intensity(1.0f),
      m_ambientColor(1.0f, 1.0f, 1.0f),
      m_diffuseColor(1.0f, 1.0f, 1.0f),
      m_specularColor(1.0f, 1.0f, 1.0f)
{}

AbstractLight::~AbstractLight() {}

void AbstractLight::setColors(const QVector3D& ambientColor,
                      const QVector3D& diffuseColor,
                      const QVector3D& specularColor)
{
    m_ambientColor  = ambientColor;
    m_diffuseColor  = diffuseColor;
    m_specularColor = specularColor;
}

void AbstractLight::setIntensity(float intensity)
{
    m_intensity = intensity;
}

void AbstractLight::setUniqueColor(const QVector3D& color)
{
    m_ambientColor  = color;
    m_diffuseColor  = color;
    m_specularColor = color;
}

void AbstractLight::setAmbientColor(const QVector3D& color)
{
    m_ambientColor = color;
}

void AbstractLight::setDiffuseColor(const QVector3D& color)
{
    m_diffuseColor = color;
}

void AbstractLight::setSpecularColor(const QVector3D& color)
{
    m_specularColor = color;
}

QVector3D AbstractLight::ambientColor() const
{
    return m_ambientColor;
}

QVector3D AbstractLight::diffuseColor() const
{
    return m_diffuseColor;
}

QVector3D AbstractLight::specularColor() const
{
    return m_specularColor;
}
