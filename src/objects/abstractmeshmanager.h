#ifndef ABSTRACTMESHMANAGER_H
#define ABSTRACTMESHMANAGER_H

#include <string>

#include <QVector>
#include <QVector3D>
#include <QVector4D>

using namespace std;

class Mesh;

class AbstractMeshManager
{

public:
    AbstractMeshManager();

    virtual Mesh* getMesh(const string& name) = 0;

    virtual Mesh* addMesh(const string& name,
                          const QVector<QVector3D>& positions,
                          const QVector<QVector4D>& colors,
                          const QVector<QVector2D>& texCoords,
                          const QVector<QVector3D>& normals,
                          const QVector<QVector3D>& tangents) = 0;

};

#endif // ABSTRACTMESHMANAGER_H
