#include "meshmanager.h"
#include "mesh.h"

#include <QDebug>
#include <QString>

MeshManager::MeshManager(const QOpenGLShaderProgramPtr& shader)
    : m_shader(shader)
{}

MeshManager::~MeshManager() {}

Mesh* MeshManager::getMesh(const string& name)
{
    if(m_meshes.find(name) != m_meshes.end())
    {
        qDebug() << "Mesh " << QString::fromStdString(name) << " found";
        return m_meshes[name].get();
    }

    qDebug() << "Mesh " << QString::fromStdString(name) << " not found";

    return nullptr;
}

Mesh* MeshManager::addMesh(const string &name,
                           const QVector<QVector3D> &positions,
                           const QVector<QVector4D> &colors,
                           const QVector<QVector2D> &texCoords,
                           const QVector<QVector3D> &normals,
                           const QVector<QVector3D> &tangents)
{
    if(m_meshes.find(name) != m_meshes.end() && m_meshes[name].get() != nullptr)
    {
        qDebug() << "Mesh " << QString::fromStdString(name) << " already exists";
        return m_meshes[name].get();
    }

    qDebug() << "Add " << QString::fromStdString(name) << " in meshes list";
    m_meshes[name] = unique_ptr<Mesh>(new Mesh(name,
                                               positions,
                                               colors,
                                               texCoords,
                                               normals,
                                               tangents,
                                               m_shader));

    return m_meshes[name].get();
}
