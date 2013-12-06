#include "meshmanager.h"
#include "mesh.h"

MeshManager::MeshManager() {}
MeshManager::~MeshManager() {}

shared_ptr<Mesh> MeshManager::getMesh(const string& name)
{
    if(m_meshes.find(name) != m_meshes.end())
    {
        return m_meshes[name];
    }

    return shared_ptr<Mesh>(nullptr);
}

shared_ptr<Mesh> MeshManager::addMesh(const string& name,
                                      unsigned int numIndices,
                                      unsigned int baseVertex,
                                      unsigned int baseIndex)
{
    if(m_meshes.find(name) != m_meshes.end() && m_meshes[name].get() != nullptr)
    {
        return m_meshes[name];
    }

    m_meshes[name] = make_shared<Mesh>(name, numIndices, baseVertex, baseIndex);

    return m_meshes[name];
}
