#ifndef ABSTRACTMESHMANAGER_H
#define ABSTRACTMESHMANAGER_H

#include <string>
#include <memory>

#include <QVector>
#include <QVector3D>

using namespace std;

class Mesh;

class AbstractMeshManager
{

public:
    AbstractMeshManager();
    virtual ~AbstractMeshManager() = 0;

    virtual shared_ptr<Mesh> getMesh(const string& name) = 0;

    virtual shared_ptr<Mesh> addMesh(const string& name,
                                     unsigned int numIndices,
                                     unsigned int baseVertex,
                                     unsigned int baseIndex) = 0;

};

#endif // ABSTRACTMESHMANAGER_H
