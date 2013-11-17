#ifndef MESH_H
#define MESH_H

#include <vector>
#include <string>
#include <memory>

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include <qopengl.h>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QSharedPointer>

#define INVALID_MATERIAL 0xFFFFFFFF

typedef QSharedPointer<QOpenGLShaderProgram> QOpenGLShaderProgramPtr;

using namespace std;

struct Vertex
{
    QVector3D m_pos;
    QVector2D m_tex;
    QVector3D m_normal;

    Vertex() {}

    Vertex(const QVector3D& pos, const QVector2D& tex, const QVector3D& normal)
    {
        m_pos    = pos;
        m_tex    = tex;
        m_normal = normal;
    }
};

class Texture;
class QOpenGLFunctions;

class Mesh
{

public:
    Mesh();
    ~Mesh();

    void Init(const QOpenGLShaderProgramPtr& shader);
    void LoadMesh(const string& Filename);
    void Render();

private:
    void InitFromScene(const aiScene* pScene, const string& Filename);
    void InitMaterials(const aiScene* pScene, const string& Filename);
    void InitMesh(unsigned int Index, const aiMesh* paiMesh);

    struct MeshEntry
    {
        MeshEntry();
        ~MeshEntry();

        void Init(const QVector<Vertex>& Vertices, const QVector<unsigned int>& Indices);

        QOpenGLBuffer m_vertexPositionBuffer;
        QOpenGLBuffer m_vertexIndexBuffer;

        unsigned int NumIndices;
        unsigned int MaterialIndex;
    };

    QOpenGLFunctions*       m_funcs;
    QOpenGLShaderProgramPtr m_shader;

    vector<MeshEntry> m_Entries;
    vector<unique_ptr<Texture>> m_Textures;

};

#endif // MESH_H
