#ifndef MESHMANAGER_H
#define MESHMANAGER_H

#include "abstractmeshmanager.h"

#include <map>

#include <QOpenGLShaderProgram>
#include <QSharedPointer>

typedef QSharedPointer<QOpenGLShaderProgram> QOpenGLShaderProgramPtr;

using namespace std;

class Mesh;

class MeshManager : public AbstractMeshManager
{

public:
    MeshManager(const QOpenGLShaderProgramPtr& shader);
    virtual ~MeshManager();

    virtual shared_ptr<Mesh> getMesh(const string& name);

    virtual shared_ptr<Mesh> addMesh(const string& name,
                                     const QVector<QVector3D>& positions,
                                     const QVector<QVector4D>& colors,
                                     const QVector<QVector2D>& texCoords,
                                     const QVector<QVector3D>& normals,
                                     const QVector<QVector3D>& tangents);

private:
    map<string, shared_ptr<Mesh>> m_meshes;
    QOpenGLShaderProgramPtr m_shader;
};

#endif // MESHMANAGER_H
