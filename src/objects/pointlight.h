#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "abstractlight.h"

class PointLight : public AbstractLight
{

public:
    PointLight();
    virtual ~PointLight();

    void setPosition(const QVector3D& position);

    void setAttenuation(float constantFactor,
                        float linearFactor,
                        float quadraticFactor);

    void setConstantAttenuation(float constantFactor);
    void setLinearAttenuation(float linearFactor);
    void setQuadraticAttenuation(float quadraticFactor);

    QVector3D position() const;

    virtual void render(const QOpenGLShaderProgramPtr& shader);

private:
    QVector3D m_position;

    float m_constantAttenuation;
    float m_linearAttenuation;
    float m_quadraticAttenuation;
};

#endif // POINTLIGHT_H
