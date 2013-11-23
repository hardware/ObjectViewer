#include "mesh.h"

#include "../materials/texture.h"

#include <QDebug>
#include <QOpenGLContext>
#include <QOpenGLFunctions_3_2_Core>
#include <QOpenGLVertexArrayObject>

Mesh::Mesh()
    : m_funcs(0),
      m_vao(new QOpenGLVertexArrayObject),
      m_vertexPositionBuffer(QOpenGLBuffer::VertexBuffer),
      m_vertexTexCoordBuffer(QOpenGLBuffer::VertexBuffer),
      m_vertexNormalBuffer(QOpenGLBuffer::VertexBuffer),
      m_vertexTangentBuffer(QOpenGLBuffer::VertexBuffer),
      m_indexBuffer(QOpenGLBuffer::IndexBuffer)
{}

Mesh::~Mesh()
{
    m_vertexPositionBuffer.destroy();
    m_vertexTexCoordBuffer.destroy();
    m_vertexNormalBuffer.destroy();
    m_vertexTangentBuffer.destroy();
    m_indexBuffer.destroy();

    m_vao->destroy();
}

Mesh::MeshEntry::MeshEntry()
    : numIndices(0),
      baseVertex(0),
      baseIndex(0),
      materialIndex(INVALID_MATERIAL)
{}

Mesh::MeshEntry::~MeshEntry() {}

void Mesh::init(const QOpenGLShaderProgramPtr& shader)
{
    QOpenGLContext* context = QOpenGLContext::currentContext();

    Q_ASSERT(context);

    m_funcs = context->versionFunctions<QOpenGLFunctions_3_2_Core>();
    m_funcs->initializeOpenGLFunctions();

    m_shader = shader;
}

void Mesh::loadMesh(const string& filename)
{
    // Create and bind the Vertex Array Object for this object
    m_vao->create();
    m_vao->bind();

    Assimp::Importer Importer;

    const aiScene* pScene = Importer.ReadFile(filename.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);

    if(pScene)
        initFromScene(pScene, filename);
    else
        qDebug() << "Error parsing : " << filename.c_str() << " -> " << Importer.GetErrorString() << endl;

    // Unbind the VAO to prevent any changes
    m_vao->release();
}

void Mesh::initFromScene(const aiScene* pScene, const string& filename)
{
    qDebug() << endl << "############### MODEL INFOS ###############";
    qDebug() << "Model path :" << filename.c_str();
    qDebug() << "Meshes :"     << pScene->mNumMeshes;
    qDebug() << "Materials :"  << pScene->mNumMaterials;

    m_entries.resize(pScene->mNumMeshes);
    m_textures.resize(pScene->mNumMaterials);

    QVector<QVector3D> positions;
    QVector<QVector3D> normals;
    QVector<QVector2D> texCoords;
    QVector<QVector3D> tangents;
    QVector<unsigned int> indices;

    unsigned int numVertices = 0;
    unsigned int numFaces    = 0;
    unsigned int numIndices  = 0;

    // Count the number of vertices and indices
    for(unsigned int i = 0; i < m_entries.size(); i++)
    {
        m_entries[i].materialIndex = pScene->mMeshes[i]->mMaterialIndex;
        m_entries[i].numIndices    = pScene->mMeshes[i]->mNumFaces * 3;
        m_entries[i].baseVertex    = numVertices;
        m_entries[i].baseIndex     = numIndices;

        numVertices += pScene->mMeshes[i]->mNumVertices;
        numFaces    += pScene->mMeshes[i]->mNumFaces;
        numIndices  += m_entries[i].numIndices;
    }

    qDebug() << "Vertices :" << numVertices;
    qDebug() << "Faces :"    << numFaces;
    qDebug() << "Indices :"  << numIndices;

    // Reserve space in the vectors for the vertex attributes and indices
    positions.reserve(numVertices);
    normals.reserve(numVertices);
    texCoords.reserve(numVertices);
    tangents.reserve(numVertices);
    indices.reserve(numIndices);

    // Initialize the meshes in the scene one by one
    for (unsigned int i = 0; i < m_entries.size() ; i++)
    {
        const aiMesh* paiMesh = pScene->mMeshes[i];
        initMesh(paiMesh, positions, texCoords, normals, tangents, indices);
    }

    initMaterials(pScene, filename);

    // Generate and populate the buffers with vertex attributes and the indices
    m_vertexPositionBuffer.create();
    m_vertexPositionBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_vertexPositionBuffer.bind();
    m_vertexPositionBuffer.allocate(positions.constData(), positions.size() * sizeof(QVector3D));

    m_vertexTexCoordBuffer.create();
    m_vertexTexCoordBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_vertexTexCoordBuffer.bind();
    m_vertexTexCoordBuffer.allocate(texCoords.constData(), texCoords.size() * sizeof(QVector2D));

    m_vertexNormalBuffer.create();
    m_vertexNormalBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_vertexNormalBuffer.bind();
    m_vertexNormalBuffer.allocate(normals.constData(), normals.size() * sizeof(QVector3D));

    m_vertexTangentBuffer.create();
    m_vertexTangentBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_vertexTangentBuffer.bind();
    m_vertexTangentBuffer.allocate(tangents.constData(), tangents.size() * sizeof(QVector3D));

    m_indexBuffer.create();
    m_indexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_indexBuffer.bind();
    m_indexBuffer.allocate(indices.constData(), indices.size() * sizeof(unsigned int));

    m_shader->bind();

    m_vertexPositionBuffer.bind();
    m_shader->enableAttributeArray("position");
    m_shader->setAttributeBuffer("position", GL_FLOAT, 0, 3);

    m_vertexTexCoordBuffer.bind();
    m_shader->enableAttributeArray("texCoord");
    m_shader->setAttributeBuffer("texCoord", GL_FLOAT, 0, 2);

    m_vertexNormalBuffer.bind();
    m_shader->enableAttributeArray("normal");
    m_shader->setAttributeBuffer("normal", GL_FLOAT, 0, 3);

    m_vertexTangentBuffer.bind();
    m_shader->enableAttributeArray("tangent");
    m_shader->setAttributeBuffer("tangent", GL_FLOAT, 0, 3);
}

void Mesh::initMesh(const aiMesh* paiMesh,
                    QVector<QVector3D>& positions,
                    QVector<QVector2D>& texCoords,
                    QVector<QVector3D>& normals,
                    QVector<QVector3D>& tangents,
                    QVector<unsigned int>& indices)
{
    const aiVector3D zero3D(0.0f, 0.0f, 0.0f);

    // Populate the vertex attribute vectors
    for(unsigned int i = 0; i < paiMesh->mNumVertices; i++)
    {
        const aiVector3D* pPos      = &(paiMesh->mVertices[i]);
        const aiVector3D* pNormal   = &(paiMesh->mNormals[i]);
        const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &zero3D;
        const aiVector3D* pTangent  = &(paiMesh->mTangents[i]);

        positions.push_back(QVector3D(pPos->x, pPos->y, pPos->z));
        texCoords.push_back(QVector2D(pTexCoord->x, pTexCoord->y));
          normals.push_back(QVector3D(pNormal->x, pNormal->y, pNormal->z));
         tangents.push_back(QVector3D(pTangent->x, pTangent->y, pTangent->z));
    }

    // Populate the index buffer
    for(unsigned int i = 0; i < paiMesh->mNumFaces; i++)
    {
        const aiFace& face = paiMesh->mFaces[i];

        Q_ASSERT(face.mNumIndices == 3);

        indices.push_back(face.mIndices[0]);
        indices.push_back(face.mIndices[1]);
        indices.push_back(face.mIndices[2]);
    }
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

    qDebug() << "###########################################" << endl;
}

void Mesh::render()
{
    m_vao->bind();

    for(unsigned int i = 0; i < m_entries.size(); i++)
    {
        const unsigned int materialIndex = m_entries[i].materialIndex;

        if(materialIndex < m_textures.size() && m_textures[materialIndex])
        {
            m_textures[materialIndex]->bind(GL_TEXTURE0);
        }

        m_funcs->glDrawElementsBaseVertex(
            GL_TRIANGLES,
            m_entries[i].numIndices,
            GL_UNSIGNED_INT,
            (void*)(sizeof(unsigned int) * m_entries[i].baseIndex),
            m_entries[i].baseVertex
        );
    }

    m_vao->release();
}
