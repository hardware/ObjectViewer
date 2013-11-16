#include "mesh.h"

#include "../materials/texture.h"

#include <QDebug>
#include <QOpenGLContext>
#include <QOpenGLFunctions>

Mesh::Mesh() : m_funcs(0) {}

Mesh::~Mesh()
{
    Clear();
}

Mesh::MeshEntry::MeshEntry()
    : m_vertexPositionBuffer(QOpenGLBuffer::VertexBuffer),
      m_vertexIndexBuffer(QOpenGLBuffer::IndexBuffer),
      NumIndices(0),
      MaterialIndex(INVALID_MATERIAL)
{}

Mesh::MeshEntry::~MeshEntry()
{
    m_vertexPositionBuffer.destroy();
    m_vertexIndexBuffer.destroy();
}

void Mesh::MeshEntry::Init(const QVector<Vertex> &Vertices, const QVector<unsigned int> &Indices)
{
    NumIndices = static_cast<unsigned int>(Indices.size());

    m_vertexPositionBuffer.create();
    m_vertexPositionBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_vertexPositionBuffer.bind();
    m_vertexPositionBuffer.allocate(Vertices.constData(), Vertices.size() * sizeof(Vertex));
    m_vertexPositionBuffer.release();

    m_vertexIndexBuffer.create();
    m_vertexIndexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_vertexIndexBuffer.bind();
    m_vertexIndexBuffer.allocate(Indices.constData(), NumIndices * sizeof(unsigned int));
    m_vertexIndexBuffer.release();
}

void Mesh::Clear()
{
    for(unsigned int i = 0; i < m_Textures.size(); i++)
    {
        SAFE_DELETE(m_Textures[i]);
    }
}

void Mesh::Init(const QOpenGLShaderProgramPtr& shader)
{
    QOpenGLContext* context = QOpenGLContext::currentContext();

    Q_ASSERT(context);

    m_funcs = context->functions();
    m_funcs->initializeOpenGLFunctions();

    m_shader = shader;
}

void Mesh::LoadMesh(const string& Filename)
{
    Clear();

    Assimp::Importer Importer;

    const aiScene* pScene = Importer.ReadFile(Filename.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals);

    if(pScene)
        InitFromScene(pScene, Filename);
    else
        qDebug() << "Error parsing : " << Filename.c_str() << " -> " << Importer.GetErrorString() << endl;
}

void Mesh::InitFromScene(const aiScene* pScene, const string& Filename)
{
    m_Entries.resize(pScene->mNumMeshes);
    m_Textures.resize(pScene->mNumMaterials);

    for(unsigned int i = 0; i < m_Entries.size(); i++)
    {
        const aiMesh* paiMesh = pScene->mMeshes[i];
        InitMesh(i, paiMesh);
    }

    InitMaterials(pScene, Filename);
}

void Mesh::InitMesh(unsigned int Index, const aiMesh* paiMesh)
{
    m_Entries[Index].MaterialIndex = paiMesh->mMaterialIndex;

    QVector<Vertex> Vertices;
    QVector<unsigned int> Indices;

    const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

    for(unsigned int i = 0; i < paiMesh->mNumVertices; i++)
    {
        const aiVector3D* pPos      = &(paiMesh->mVertices[i]);
        const aiVector3D* pNormal   = &(paiMesh->mNormals[i]);
        const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;

        Vertex v(Vector3f(pPos->x, pPos->y, pPos->z),
                 Vector2f(pTexCoord->x, pTexCoord->y),
                 Vector3f(pNormal->x, pNormal->y, pNormal->z));

        Vertices.push_back(v);
    }

    for(unsigned int i = 0; i < paiMesh->mNumFaces; i++)
    {
        const aiFace& Face = paiMesh->mFaces[i];

        Q_ASSERT(Face.mNumIndices == 3);

        Indices.push_back(Face.mIndices[0]);
        Indices.push_back(Face.mIndices[1]);
        Indices.push_back(Face.mIndices[2]);
    }

    m_Entries[Index].Init(Vertices, Indices);
}

bool Mesh::InitMaterials(const aiScene* pScene, const string& Filename)
{
    string::size_type SlashIndex = Filename.find_last_of("/");
    string Dir;

    if(SlashIndex == string::npos) Dir = ".";
    else if(SlashIndex == 0) Dir = "/";
    else Dir = Filename.substr(0, SlashIndex);

    bool Ret = true;

    for(unsigned int i = 0; i < pScene->mNumMaterials; i++)
    {
        const aiMaterial* pMaterial = pScene->mMaterials[i];

        m_Textures[i] = NULL;

        if(pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0)
        {
            aiString Path;

            if(pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
            {
                QImage image;
                string FullPath = Dir + "/" + Path.data;

                if(image.load(QString(FullPath.c_str())))
                {
                    m_Textures[i] = new Texture(image);
                    m_Textures[i]->load();

                    qDebug() << "Loaded texture :" << FullPath.c_str();
                }
                else
                {
                    qDebug() << "Error loading texture :" << FullPath.c_str();
                    Ret = false;
                }
            }
        }

        if( ! m_Textures[i] )
        {
            m_Textures[i] = new Texture(QImage(":/resources/images/white.png"));
            m_Textures[i]->load();
        }
    }

    return Ret;
}

void Mesh::Render()
{
    m_shader->bind();

    for(unsigned int i = 0; i < m_Entries.size(); i++)
    {
        m_Entries[i].m_vertexPositionBuffer.bind();

        m_shader->enableAttributeArray("Position");
        m_shader->enableAttributeArray("TexCoord");
        m_shader->enableAttributeArray("Normal");

        m_shader->setAttributeBuffer("Position", GL_FLOAT, 0,  3, sizeof(Vertex));
        m_shader->setAttributeBuffer("TexCoord", GL_FLOAT, 12, 2, sizeof(Vertex));
        m_shader->setAttributeBuffer("Normal",   GL_FLOAT, 20, 3, sizeof(Vertex));

        m_Entries[i].m_vertexIndexBuffer.bind();

        const unsigned int MaterialIndex = m_Entries[i].MaterialIndex;

        if(MaterialIndex < m_Textures.size() && m_Textures[MaterialIndex])
        {
            m_Textures[MaterialIndex]->bind(GL_TEXTURE0);
        }

        glDrawElements(GL_TRIANGLES, m_Entries[i].NumIndices, GL_UNSIGNED_INT, 0);
    }
}
