#ifndef ABSTRACTMESHMANAGER_H
#define ABSTRACTMESHMANAGER_H

#include <string>
#include <memory>

#include <QVector>
#include <QVector3D>
#include <QVector4D>

using namespace std;

class Mesh;

class AbstractMeshManager
{

public:
    AbstractMeshManager();
    virtual ~AbstractMeshManager() = 0;

    virtual shared_ptr<Mesh> getMesh(const string& name) = 0;

    virtual shared_ptr<Mesh> addMesh(const string& name,
                                     const vector<QVector3D>& positions,
                                     const vector<QVector4D>& colors,
                                     const vector<QVector2D>& texCoords,
                                     const vector<QVector3D>& normals,
                                     const vector<QVector3D>& tangents) = 0;

};

#endif // ABSTRACTMESHMANAGER_H
