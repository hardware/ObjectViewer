#include "directionallight.h"

DirectionalLight::DirectionalLight()
    : m_direction(0.0f, 0.0f, 0.0f)
{}

DirectionalLight::~DirectionalLight() {}

void DirectionalLight::setDirection(const QVector3D& direction)
{
    m_direction = direction;
}

QVector3D DirectionalLight::direction() const
{
    return m_direction;
}

void DirectionalLight::render(const QOpenGLShaderProgramPtr& shader)
{
    shader->setUniformValue("light.direction", m_direction);

    shader->setUniformValue("light.Ka", m_ambientColor);
    shader->setUniformValue("light.Kd", m_diffuseColor);
    shader->setUniformValue("light.Ks", m_specularColor);

    shader->setUniformValue("light.intensity", m_intensity);
}
