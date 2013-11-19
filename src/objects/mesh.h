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

    void init(const QOpenGLShaderProgramPtr& shader);
    void loadMesh(const string& filename);
    void render();

private:
    void initFromScene(const aiScene* pScene, const string& filename);
    void initMaterials(const aiScene* pScene, const string& filename);
    void initMesh(unsigned int index, const aiMesh* paiMesh);

    struct MeshEntry
    {
        MeshEntry();
        ~MeshEntry();

        void init(const QVector<Vertex>& vertices, const QVector<unsigned int>& indices);

        QOpenGLBuffer m_vertexPositionBuffer;
        QOpenGLBuffer m_vertexIndexBuffer;

        unsigned int numIndices;
        unsigned int materialIndex;
    };

    QOpenGLFunctions*       m_funcs;
    QOpenGLShaderProgramPtr m_shader;

    vector<MeshEntry> m_entries;
    vector<unique_ptr<Texture>> m_textures;

};

#endif // MESH_H
