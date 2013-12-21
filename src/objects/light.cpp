#include "light.h"

#include "../helpers/MathUtils.h"

Light::Light(const string& name) :
    m_name(name),
    m_type(LightType::PointLight),
    m_position(ZERO),
    m_direction(0.0f, 0.0f, 1.0f),
    m_ambientColor(Qt::black),
    m_diffuseColor(Qt::white),
    m_specularColor(Qt::black),

    QVector3D::
{
}


