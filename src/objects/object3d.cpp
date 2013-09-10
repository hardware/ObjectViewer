#include "object3d.h"

Object3D::Object3D()
    : m_position(0.0f, 0.0f, 0.0f),
      m_rotation(0.0f, 0.0f, 0.0f),
      m_scale(1.0f, 1.0f, 1.0f),
      m_modelMatrixDirty(true)
{}

void Object3D::setPosition(const QVector3D& positionVector)
{
    m_position = positionVector;

    m_modelMatrixDirty = true;
}

void Object3D::setPosition(float x, float y, float z)
{
    m_position.setX(x);
    m_position.setY(y);
    m_position.setZ(z);

    m_modelMatrixDirty = true;
}

void Object3D::setRotation(const QVector3D& rotationVector)
{
    m_rotation = rotationVector;

    m_modelMatrixDirty = true;
}

void Object3D::setRotation(float x, float y, float z)
{
    m_rotation.setX(x);
    m_rotation.setY(y);
    m_rotation.setZ(z);

    m_modelMatrixDirty = true;
}

void Object3D::setScale(const QVector3D& scale)
{
    m_scale = scale;

    m_modelMatrixDirty = true;
}

void Object3D::setScale(float x, float y, float z)
{
    m_scale.setX(x);
    m_scale.setY(y);
    m_scale.setZ(z);

    m_modelMatrixDirty = true;
}

void Object3D::setScale(float scaleFactor)
{
    m_scale.setX(scaleFactor);
    m_scale.setY(scaleFactor);
    m_scale.setZ(scaleFactor);

    m_modelMatrixDirty = true;
}

void Object3D::translateX(float x)
{
    m_position.setX(x);
    m_modelMatrixDirty = true;
}

void Object3D::translateY(float y)
{
    m_position.setY(y);
    m_modelMatrixDirty = true;
}

void Object3D::translateZ(float z)
{
    m_position.setZ(z);
    m_modelMatrixDirty = true;
}

void Object3D::rotateX(float x)
{
    m_rotation.setX(x);
    m_modelMatrixDirty = true;
}

void Object3D::rotateY(float y)
{
    m_rotation.setY(y);
    m_modelMatrixDirty = true;
}

void Object3D::rotateZ(float z)
{
    m_rotation.setZ(z);
    m_modelMatrixDirty = true;
}

void Object3D::scaleX(float x)
{
    m_scale.setX(x);
    m_modelMatrixDirty = true;
}

void Object3D::scaleY(float y)
{
    m_scale.setY(y);
    m_modelMatrixDirty = true;
}

void Object3D::scaleZ(float z)
{
    m_scale.setZ(z);
    m_modelMatrixDirty = true;
}

const QVector3D& Object3D::position() const
{
    return m_position;
}

const QVector3D& Object3D::rotation() const
{
    return m_rotation;
}

const QVector3D& Object3D::scale() const
{
    return m_scale;
}

const QMatrix4x4& Object3D::modelMatrix()
{
    if(m_modelMatrixDirty)
    {
        m_modelMatrix.setToIdentity();

        m_modelMatrix.translate(m_position);
        m_modelMatrix.rotate(m_rotation.x(), 1.0f, 0.0f, 0.0f);
        m_modelMatrix.rotate(m_rotation.y(), 0.0f, 1.0f, 0.0f);
        m_modelMatrix.rotate(m_rotation.z(), 0.0f, 0.0f, 1.0f);
        m_modelMatrix.scale(m_scale);

        m_modelMatrixDirty = false;
    }

    return m_modelMatrix;
}

void Object3D::setObjectXPosition(int x)
{
    m_position.setX(x/100.0f);
    m_modelMatrixDirty = true;
}

void Object3D::setObjectYPosition(int y)
{
    m_position.setY(y/100.0f);
    m_modelMatrixDirty = true;
}

void Object3D::setObjectZPosition(int z)
{
    m_position.setZ(z/100.0f);
    m_modelMatrixDirty = true;
}

void Object3D::setObjectXRotation(int x)
{
    m_rotation.setX(x);
    m_modelMatrixDirty = true;
}

void Object3D::setObjectYRotation(int y)
{
    m_rotation.setY(y);
    m_modelMatrixDirty = true;
}

void Object3D::setObjectZRotation(int z)
{
    m_rotation.setZ(z);
    m_modelMatrixDirty = true;
}

void Object3D::reset()
{
    setPosition(0.0f, 0.0f, 0.0f);
    setRotation(0.0f, 0.0f, 0.0f);
    setScale(1.0f, 1.0f, 1.0f);
}
