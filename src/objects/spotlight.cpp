#include "spotlight.h"

SpotLight::SpotLight()
    : m_cutOff(0.0f)
{}

SpotLight::~SpotLight() {}

//void SpotLight::setSpotlightRange(float innerAngle, float outerAngle, float falloff)
//{
//    m_innerAngle = innerAngle;
//    m_outerAngle = outerAngle;
//    m_falloff    = falloff;
//}

//void SpotLight::setInnerAngle(float innerAngle)
//{
//    m_innerAngle = innerAngle;
//}

//void SpotLight::setOuterAngle(float outerAngle)
//{
//    m_outerAngle = outerAngle;
//}

//void SpotLight::setFalloff(float falloff)
//{
//    m_falloff = falloff;
//}

//void SpotLight::setNearClipDistance(float nearClip)
//{
//    m_nearClip = nearClip;
//}

//float SpotLight::getInnerAngle() const
//{
//    return m_innerAngle;
//}

//float SpotLight::getOuterAngle() const
//{
//    return m_outerAngle;
//}

//float SpotLight::getFalloff() const
//{
//    return m_falloff;
//}

//float SpotLight::getNearClipDistance () const
//{
//    return m_nearClip;
//}

void SpotLight::setCutOff(float cutOff)
{
    m_cutOff = cutOff;
}

void SpotLight::render(const QOpenGLShaderProgramPtr& shader)
{
    shader->setUniformValue("light.position", m_position);
    shader->setUniformValue("light.direction", m_direction);

    shader->setUniformValue("light.Ka", m_ambientColor);
    shader->setUniformValue("light.Kd", m_diffuseColor);
    shader->setUniformValue("light.Ks", m_specularColor);

    shader->setUniformValue("light.constantAttenuation",  m_constantAttenuation);
    shader->setUniformValue("light.linearAttenuation",    m_linearAttenuation);
    shader->setUniformValue("light.quadraticAttenuation", m_quadraticAttenuation);

    shader->setUniformValue("light.intensity", m_intensity);
    shader->setUniformValue("light.cutOff", m_cutOff);
}
