#include "directionallight.h"

DirectionalLight::DirectionalLight()
    : m_direction(0.0f, 0.0f, 0.0f)
{}

DirectionalLight::~DirectionalLight() {}

void DirectionalLight::setDirection(float x, float y, float z)
{
    m_direction.setX(x);
    m_direction.setY(y);
    m_direction.setZ(z);
}

void DirectionalLight::setDirection(const QVector3D& direction)
{
    m_direction = direction;
}

const QVector3D& DirectionalLight::getDirection() const
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
