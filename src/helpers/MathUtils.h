#ifndef MATHUTILS_H
#define MATHUTILS_H

#include <QVector2D>
#include <QVector3D>
#include <QVector4D>

namespace Math
{
    namespace Vector2D
    {
        const QVector2D ZERO = QVector2D(0.0f, 0.0f);

        const QVector2D UNIT_X = QVector2D(1.0f, 0.0f);
        const QVector2D UNIT_Y = QVector2D(0.0f, 1.0f);

        const QVector2D NEGATIVE_UNIT_X = QVector2D(-1.0f,  0.0f);
        const QVector2D NEGATIVE_UNIT_Y = QVector2D( 0.0f, -1.0f);

        const QVector2D UNIT_SCALE = QVector2D(1.0f, 1.0f);
    }

    namespace Vector3D
    {
        const QVector3D ZERO = QVector3D(0.0f, 0.0f, 0.0f);

        const QVector3D UNIT_X = QVector3D(1.0f, 0.0f, 0.0f);
        const QVector3D UNIT_Y = QVector3D(0.0f, 1.0f, 0.0f);
        const QVector3D UNIT_Z = QVector3D(0.0f, 0.0f, 1.0f);

        const QVector3D NEGATIVE_UNIT_X = QVector3D(-1.0f,  0.0f,  0.0f);
        const QVector3D NEGATIVE_UNIT_Y = QVector3D( 0.0f, -1.0f,  0.0f);
        const QVector3D NEGATIVE_UNIT_Z = QVector3D( 0.0f,  0.0f, -1.0f);

        const QVector3D UNIT_SCALE = QVector3D(1.0f, 1.0f, 1.0f);
    }

    namespace Vector4D
    {
        const QVector4D ZERO = QVector4D(0.0f, 0.0f, 0.0f, 0.0f);
    }
}

#endif // MATHUTILS_H
