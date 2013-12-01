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
                                     const vector<QVector3D>& positions,
                                     const vector<QVector4D>& colors,
                                     const vector<QVector2D>& texCoords,
                                     const vector<QVector3D>& normals,
                                     const vector<QVector3D>& tangents);

private:
    map<string, shared_ptr<Mesh>> m_meshes;
    QOpenGLShaderProgramPtr m_shader;
};

#endif // MESHMANAGER_H
