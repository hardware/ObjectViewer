#ifndef MESHMANAGER_H
#define MESHMANAGER_H

#include "abstractmeshmanager.h"

#include <memory>
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

    virtual Mesh* getMesh(const string& name);

    virtual Mesh* addMesh(const string& name,
                          const QVector<QVector3D>& positions,
                          const QVector<QVector4D>& colors,
                          const QVector<QVector2D>& texCoords,
                          const QVector<QVector3D>& normals,
                          const QVector<QVector3D>& tangents);

private:
    map<string, unique_ptr<Mesh>> m_meshes;
    QOpenGLShaderProgramPtr m_shader;
};

#endif // MESHMANAGER_H
