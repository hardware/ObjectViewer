#include "meshmanager.h"
#include "mesh.h"

#include <QDebug>
#include <QString>

MeshManager::MeshManager(const QOpenGLShaderProgramPtr& shader)
    : m_shader(shader)
{}

MeshManager::~MeshManager() {}

shared_ptr<Mesh> MeshManager::getMesh(const string& name)
{
    if(m_meshes.find(name) != m_meshes.end())
    {
        qDebug() << "Mesh " << QString::fromStdString(name) << " found";
        return m_meshes[name];
    }

    qDebug() << "Mesh " << QString::fromStdString(name) << " not found";

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
        qDebug() << "Mesh " << QString::fromStdString(name) << " already exists";
        return m_meshes[name];
    }

    qDebug() << "Add " << QString::fromStdString(name) << " in meshes list";
    m_meshes[name] = shared_ptr<Mesh>(new Mesh(name,
                                               positions,
                                               colors,
                                               texCoords,
                                               normals,
                                               tangents,
                                               m_shader));

    return m_meshes[name];
}
