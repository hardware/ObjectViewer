#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H

#include "pointlight.h"
#include "directionallight.h"

class SpotLight : public PointLight, public DirectionalLight
{

public:
    SpotLight();
    virtual ~SpotLight();

//    void setSpotlightRange(float innerAngle, float outerAngle, float falloff = 1.0f);

//    void setInnerAngle(float innerAngle);
//    void setOuterAngle(float outerAngle);
//    void setFalloff(float falloff);
//    void setNearClipDistance(float nearClip);

//    float getInnerAngle() const;
//    float getOuterAngle() const;
//    float getFalloff() const;
//    float getNearClipDistance () const;

    void setCutOff(float cutOff);

    virtual void render(const QOpenGLShaderProgramPtr& shader);

private:
//    float m_outerAngle;
//    float m_innerAngle;
//    float m_falloff;
//    float m_nearClip;

    float m_cutOff;

};

#endif // SPOTLIGHT_H
