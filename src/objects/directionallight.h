#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H

#include "abstractlight.h"

class DirectionalLight : public AbstractLight
{

public:
    DirectionalLight();
    virtual ~DirectionalLight();

    void setDirection(const QVector3D& direction);
    QVector3D direction() const;

    virtual void render(const QOpenGLShaderProgramPtr& shader);

private:
    QVector3D m_direction;

};

#endif // DIRECTIONALLIGHT_H
