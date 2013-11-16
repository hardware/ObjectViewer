#ifndef MESH_H
#define MESH_H

#include <map>
#include <vector>
#include <string>

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "../helpers/util.h"
#include "../helpers/math_3d.h"

#include <qopengl.h>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QSharedPointer>

typedef QSharedPointer<QOpenGLShaderProgram> QOpenGLShaderProgramPtr;

using namespace std;

struct Vertex
{
    Vector3f m_pos;
    Vector2f m_tex;
    Vector3f m_normal;

    Vertex() {}

    Vertex(const Vector3f& pos, const Vector2f& tex, const Vector3f& normal)
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

    void Init(const QOpenGLShaderProgramPtr &shader);
    void LoadMesh(const string& Filename);
    void Render();

private:
    void InitFromScene(const aiScene* pScene, const string& Filename);
    bool InitMaterials(const aiScene* pScene, const string& Filename);
    void InitMesh(unsigned int Index, const aiMesh* paiMesh);
    void Clear();

    #define INVALID_MATERIAL 0xFFFFFFFF

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
    vector<Texture*>  m_Textures;

};

#endif // MESH_H
