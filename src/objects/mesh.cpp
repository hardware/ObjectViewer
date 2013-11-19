#include "mesh.h"

#include "../materials/texture.h"

#include <QDebug>
#include <QOpenGLContext>
#include <QOpenGLFunctions>

Mesh::Mesh() : m_funcs(0) {}

Mesh::~Mesh() {}

Mesh::MeshEntry::MeshEntry()
    : m_vertexPositionBuffer(QOpenGLBuffer::VertexBuffer),
      m_vertexIndexBuffer(QOpenGLBuffer::IndexBuffer),
      numIndices(0),
      materialIndex(INVALID_MATERIAL)
{}

Mesh::MeshEntry::~MeshEntry()
{
    m_vertexPositionBuffer.destroy();
    m_vertexIndexBuffer.destroy();
}

void Mesh::MeshEntry::init(const QVector<Vertex>& vertices, const QVector<unsigned int>& indices)
{
    numIndices = static_cast<unsigned int>(indices.size());

    m_vertexPositionBuffer.create();
    m_vertexPositionBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_vertexPositionBuffer.bind();
    m_vertexPositionBuffer.allocate(vertices.constData(), vertices.size() * sizeof(Vertex));
    m_vertexPositionBuffer.release();

    m_vertexIndexBuffer.create();
    m_vertexIndexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_vertexIndexBuffer.bind();
    m_vertexIndexBuffer.allocate(indices.constData(), numIndices * sizeof(unsigned int));
    m_vertexIndexBuffer.release();
}

void Mesh::init(const QOpenGLShaderProgramPtr& shader)
{
    QOpenGLContext* context = QOpenGLContext::currentContext();

    Q_ASSERT(context);

    m_funcs = context->functions();
    m_funcs->initializeOpenGLFunctions();

    m_shader = shader;
}

void Mesh::loadMesh(const string& filename)
{
    Assimp::Importer Importer;

    const aiScene* pScene = Importer.ReadFile(filename.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);

    if(pScene)
        initFromScene(pScene, filename);
    else
        qDebug() << "Error parsing : " << filename.c_str() << " -> " << Importer.GetErrorString() << endl;
}

void Mesh::initFromScene(const aiScene* pScene, const string& filename)
{
    m_entries.resize(pScene->mNumMeshes);
    m_textures.resize(pScene->mNumMaterials);

    for(unsigned int i = 0; i < m_entries.size(); i++)
    {
        const aiMesh* paiMesh = pScene->mMeshes[i];
        initMesh(i, paiMesh);
    }

    initMaterials(pScene, filename);
}

void Mesh::initMesh(unsigned int index, const aiMesh* paiMesh)
{
    m_entries[index].materialIndex = paiMesh->mMaterialIndex;

    QVector<Vertex> vertices;
    QVector<unsigned int> indices;

    const aiVector3D zero3D(0.0f, 0.0f, 0.0f);

    for(unsigned int i = 0; i < paiMesh->mNumVertices; i++)
    {
        const aiVector3D* pPos      = &(paiMesh->mVertices[i]);
        const aiVector3D* pNormal   = &(paiMesh->mNormals[i]);
        const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &zero3D;

        Vertex v(QVector3D(pPos->x, pPos->y, pPos->z),
                 QVector2D(pTexCoord->x, pTexCoord->y),
                 QVector3D(pNormal->x, pNormal->y, pNormal->z));

        vertices.push_back(v);
    }

    for(unsigned int i = 0; i < paiMesh->mNumFaces; i++)
    {
        const aiFace& face = paiMesh->mFaces[i];

        Q_ASSERT(face.mNumIndices == 3);

        indices.push_back(face.mIndices[0]);
        indices.push_back(face.mIndices[1]);
        indices.push_back(face.mIndices[2]);
    }

    m_entries[index].init(vertices, indices);
}

void Mesh::initMaterials(const aiScene* pScene, const string& filename)
{
    string::size_type slashIndex = filename.find_last_of("/");
    string dir;

    if(slashIndex == string::npos) dir = ".";
    else if(slashIndex == 0) dir = "/";
    else dir = filename.substr(0, slashIndex);

    for(unsigned int i = 0; i < pScene->mNumMaterials; i++)
    {
        const aiMaterial* pMaterial = pScene->mMaterials[i];

        m_textures[i] = NULL;

        if(pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0)
        {
            aiString path;

            if(pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
            {
                QImage image;
                string fullPath = dir + "/" + path.data;

                if(image.load(QString(fullPath.c_str())))
                {
                    m_textures.insert(m_textures.begin() + i, unique_ptr<Texture>(new Texture(image)));
                    m_textures[i]->load();

                    qDebug() << "Loaded texture :" << fullPath.c_str();
                }
                else
                {
                    qDebug() << "Error loading texture :" << fullPath.c_str();
                }
            }
        }

        if( ! m_textures[i] )
        {
            m_textures.insert(m_textures.begin() + i, unique_ptr<Texture>(new Texture(QImage(":/resources/images/white.png"))));
            m_textures[i]->load();
        }
    }
}

void Mesh::render()
{
    m_shader->bind();

    for(unsigned int i = 0; i < m_entries.size(); i++)
    {
        m_entries[i].m_vertexPositionBuffer.bind();

        m_shader->enableAttributeArray("position");
        m_shader->enableAttributeArray("texCoord");
        m_shader->enableAttributeArray("normal");

        m_shader->setAttributeBuffer("position", GL_FLOAT, 0,  3, sizeof(Vertex));
        m_shader->setAttributeBuffer("texCoord", GL_FLOAT, 12, 2, sizeof(Vertex));
        m_shader->setAttributeBuffer("normal",   GL_FLOAT, 20, 3, sizeof(Vertex));

        m_entries[i].m_vertexIndexBuffer.bind();

        const unsigned int materialIndex = m_entries[i].materialIndex;

        if(materialIndex < m_textures.size() && m_textures[materialIndex])
        {
            m_textures[materialIndex]->bind(GL_TEXTURE0);
        }

        glDrawElements(GL_TRIANGLES, m_entries[i].numIndices, GL_UNSIGNED_INT, 0);
    }
}
