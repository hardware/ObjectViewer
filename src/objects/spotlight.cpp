#include "spotlight.h"

SpotLight::SpotLight() {}
SpotLight::~SpotLight() {}

void SpotLight::setSpotlightRange(float innerAngle, float outerAngle, float falloff)
{
    m_innerAngle = innerAngle;
    m_outerAngle = outerAngle;
    m_falloff    = falloff;
}

void SpotLight::setInnerAngle(float innerAngle)
{
    m_innerAngle = innerAngle;
}

void SpotLight::setOuterAngle(float outerAngle)
{
    m_outerAngle = outerAngle;
}

void SpotLight::setFalloff(float falloff)
{
    m_falloff = falloff;
}

void SpotLight::setNearClipDistance(float nearClip)
{
    m_nearClip = nearClip;
}

float SpotLight::getInnerAngle() const
{
    return m_innerAngle;
}

float SpotLight::getOuterAngle() const
{
    return m_outerAngle;
}

float SpotLight::getFalloff() const
{
    return m_falloff;
}

float SpotLight::getNearClipDistance () const
{
    return m_nearClip;
}

void SpotLight::render(const QOpenGLShaderProgramPtr &shader)
{
    Q_UNUSED(shader);
}
