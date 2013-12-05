#include "abstractlight.h"

AbstractLight::AbstractLight()
    : m_intensity(1.0f),
      m_ambientColor(0.0f, 0.0f, 0.0f),
      m_diffuseColor(0.0f, 0.0f, 0.0f),
      m_specularColor(0.0f, 0.0f, 0.0f)
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

void AbstractLight::setAmbientColor(float red, float green, float blue)
{
    m_ambientColor.setX(red);
    m_ambientColor.setY(green);
    m_ambientColor.setZ(blue);
}

void AbstractLight::setDiffuseColor(float red, float green, float blue)
{
    m_diffuseColor.setX(red);
    m_diffuseColor.setY(green);
    m_diffuseColor.setZ(blue);
}

void AbstractLight::setSpecularColor(float red, float green, float blue)
{
    m_specularColor.setX(red);
    m_specularColor.setY(green);
    m_specularColor.setZ(blue);
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

const QVector3D& AbstractLight::ambientColor() const
{
    return m_ambientColor;
}

const QVector3D& AbstractLight::diffuseColor() const
{
    return m_diffuseColor;
}

const QVector3D& AbstractLight::specularColor() const
{
    return m_specularColor;
}
