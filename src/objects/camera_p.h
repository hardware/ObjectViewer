#ifndef CAMERA_P_H
#define CAMERA_P_H

#include <QMatrix4x4>
#include <QVector3D>

#include "../helpers/MathUtils.h"

using namespace Math;

class CameraPrivate
{

public:
    CameraPrivate(Camera* c)
        : q_ptr(c),
          m_position(Vector3D::UNIT_Z),
          m_upVector(Vector3D::UNIT_Y),
          m_viewCenter(Vector3D::ZERO),
          m_cameraToCenter(Vector3D::NEGATIVE_UNIT_Z),
          m_projectionType(Camera::PerspectiveProjection),
          m_nearPlane(0.1f),
          m_farPlane(1024.0f),
          m_fieldOfView(75.0f),
          m_aspectRatio(1.0f),
          m_left(-0.5f),
          m_right(0.5f),
          m_bottom(-0.5f),
          m_top(0.5f),
          m_viewMatrixDirty(true),
          m_viewProjectionMatrixDirty(true)
    {
        updateOrthogonalProjection();
    }

    ~CameraPrivate() {}

    inline void updatePerspectiveProjection()
    {
        m_projectionMatrix.setToIdentity();
        m_projectionMatrix.perspective(m_fieldOfView, m_aspectRatio, m_nearPlane, m_farPlane);
        m_viewProjectionMatrixDirty = true;
    }

    inline void updateOrthogonalProjection()
    {
        m_projectionMatrix.setToIdentity();
        m_projectionMatrix.ortho(m_left, m_right, m_bottom, m_top, m_nearPlane, m_farPlane);
        m_viewProjectionMatrixDirty = true;
    }

    Q_DECLARE_PUBLIC(Camera)

    Camera* q_ptr;

    QVector3D m_position;
    QVector3D m_upVector;
    QVector3D m_viewCenter;
    QVector3D m_cameraToCenter;

    Camera::ProjectionType m_projectionType;

    float m_nearPlane;
    float m_farPlane;
    float m_fieldOfView;
    float m_aspectRatio;

    float m_left;
    float m_right;
    float m_bottom;
    float m_top;

    mutable QMatrix4x4 m_viewMatrix;
    mutable QMatrix4x4 m_projectionMatrix;
    mutable QMatrix4x4 m_viewProjectionMatrix;

    mutable bool m_viewMatrixDirty;
    mutable bool m_viewProjectionMatrixDirty;

};

#endif // CAMERA_P_H
