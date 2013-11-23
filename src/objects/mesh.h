#ifndef MESH_H
#define MESH_H

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
    QVector3D m_tangent;

    Vertex() {}

    Vertex(const QVector3D& pos, const QVector2D& tex, const QVector3D& normal , const QVector3D& tangent)
    {
        m_pos     = pos;
        m_tex     = tex;
        m_normal  = normal;
        m_tangent = tangent;
    }
};

class Texture;
class QOpenGLFunctions_3_2_Core;
class QOpenGLVertexArrayObject;

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

    void initMesh(const aiMesh* paiMesh,
                  QVector<QVector3D>& positions,
                  QVector<QVector2D>& texCoords,
                  QVector<QVector3D>& normals,
                  QVector<QVector3D>& tangents,
                  QVector<unsigned int>& indices);

    QOpenGLVertexArrayObject* m_vao;

    QOpenGLBuffer m_vertexPositionBuffer;
    QOpenGLBuffer m_vertexTexCoordBuffer;
    QOpenGLBuffer m_vertexNormalBuffer;
    QOpenGLBuffer m_vertexTangentBuffer;
    QOpenGLBuffer m_indexBuffer;

    struct MeshEntry
    {
        MeshEntry();
        ~MeshEntry();

        unsigned int numIndices;
        unsigned int baseVertex;
        unsigned int baseIndex;
        unsigned int materialIndex;
    };

    QOpenGLFunctions_3_2_Core* m_funcs;
    QOpenGLShaderProgramPtr    m_shader;

    vector<MeshEntry> m_entries;
    vector<unique_ptr<Texture>> m_textures;

};

#endif // MESH_H
