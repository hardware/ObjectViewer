#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "abstractlight.h"

class PointLight : public virtual AbstractLight
{

public:
    PointLight();
    virtual ~PointLight();

    void setPosition(float x, float y, float z);
    void setPosition(const QVector3D& position);

    void setAttenuation(float constantFactor,
                        float linearFactor,
                        float quadraticFactor);

    void setConstantAttenuation(float constantFactor);
    void setLinearAttenuation(float linearFactor);
    void setQuadraticAttenuation(float quadraticFactor);

    float getConstantAttenuation() const;
    float getLinearAttenuation() const;
    float getQuadraticAttenuation() const;

    const QVector3D& getPosition() const;

    virtual void render(const QOpenGLShaderProgramPtr& shader);

private:
    QVector3D m_position;

    float m_constantAttenuation;
    float m_linearAttenuation;
    float m_quadraticAttenuation;
};

#endif // POINTLIGHT_H
