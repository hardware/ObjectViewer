#include "pointlight.h"

PointLight::PointLight()
    : m_position(0.0f, 0.0f, 0.0f),
      m_constantAttenuation(1.0f),
      m_linearAttenuation(0.0f),
      m_quadraticAttenuation(0.0f)
{}

PointLight::~PointLight() {}

void PointLight::setPosition(float x, float y, float z)
{
    m_position.setX(x);
    m_position.setY(y);
    m_position.setZ(z);
}

void PointLight::setPosition(const QVector3D& position)
{
    m_position = position;
}

void PointLight::setAttenuation(float constantFactor, float linearFactor, float quadraticFactor)
{
    m_constantAttenuation  = constantFactor;
    m_linearAttenuation    = linearFactor;
    m_quadraticAttenuation = quadraticFactor;
}

void PointLight::setConstantAttenuation(float constantFactor)
{
    m_constantAttenuation = constantFactor;
}

void PointLight::setLinearAttenuation(float linearFactor)
{
    m_linearAttenuation = linearFactor;
}

void PointLight::setQuadraticAttenuation(float quadraticFactor)
{
    m_quadraticAttenuation = quadraticFactor;
}

float PointLight::getConstantAttenuation() const
{
    return m_constantAttenuation;
}

float PointLight::getLinearAttenuation() const
{
    return m_linearAttenuation;
}

float PointLight::getQuadraticAttenuation() const
{
    return m_quadraticAttenuation;
}

const QVector3D& PointLight::getPosition() const
{
    return m_position;
}

void PointLight::render(const QOpenGLShaderProgramPtr& shader)
{
    shader->setUniformValue("light.position", m_position);

    shader->setUniformValue("light.Ka", m_ambientColor);
    shader->setUniformValue("light.Kd", m_diffuseColor);
    shader->setUniformValue("light.Ks", m_specularColor);

//    shader->setUniformValue("light.constantAttenuation",  m_constantAttenuation);
//    shader->setUniformValue("light.linearAttenuation",    m_linearAttenuation);
//    shader->setUniformValue("light.quadraticAttenuation", m_quadraticAttenuation);

    shader->setUniformValue("light.intensity", m_intensity);
}
