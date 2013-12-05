#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H

#include "abstractlight.h"

class DirectionalLight : public virtual AbstractLight
{

public:
    DirectionalLight();
    virtual ~DirectionalLight();

    void setDirection(float x, float y, float z);
    void setDirection(const QVector3D& direction);

    const QVector3D& getDirection() const;

    virtual void render(const QOpenGLShaderProgramPtr& shader);

protected:
    QVector3D m_direction;

};

#endif // DIRECTIONALLIGHT_H
