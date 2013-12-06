#ifndef MESHMANAGER_H
#define MESHMANAGER_H

#include "abstractmeshmanager.h"

#include <map>

using namespace std;

class Mesh;

class MeshManager : public AbstractMeshManager
{

public:
    MeshManager();
    virtual ~MeshManager();

    virtual shared_ptr<Mesh> getMesh(const string& name);

    virtual shared_ptr<Mesh> addMesh(const string& name,
                                     unsigned int numIndices,
                                     unsigned int baseVertex,
                                     unsigned int baseIndex);

private:
    map<string, shared_ptr<Mesh>> m_meshes;

};

#endif // MESHMANAGER_H
