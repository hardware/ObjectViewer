#include "meshmanager.h"
#include "mesh.h"

MeshManager::MeshManager(const QOpenGLShaderProgramPtr& shader)
    : m_shader(shader)
{}

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
                                      const vector<QVector3D>& positions,
                                      const vector<QVector4D>& colors,
                                      const vector<QVector2D>& texCoords,
                                      const vector<QVector3D>& normals,
                                      const vector<QVector3D>& tangents)
{
    if(m_meshes.find(name) != m_meshes.end() && m_meshes[name].get() != nullptr)
    {
        return m_meshes[name];
    }

    m_meshes[name] = make_shared<Mesh>(name,
                                       positions,
                                       colors,
                                       texCoords,
                                       normals,
                                       tangents,
                                       m_shader);

    return m_meshes[name];
}
