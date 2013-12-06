#include "modelloader.h"

#include <QDebug>
#include <QString>

ModelLoader::ModelLoader()
    : m_vertexPositionBuffer(QOpenGLBuffer::VertexBuffer),
      m_vertexColorBuffer(QOpenGLBuffer::VertexBuffer),
      m_vertexTexCoordBuffer(QOpenGLBuffer::VertexBuffer),
      m_vertexNormalBuffer(QOpenGLBuffer::VertexBuffer),
      m_vertexTangentBuffer(QOpenGLBuffer::VertexBuffer),
      m_indexBuffer(QOpenGLBuffer::IndexBuffer),
      m_vao(QOpenGLVertexArrayObjectPtr(new QOpenGLVertexArrayObject()))
{}

ModelLoader::~ModelLoader()
{
    m_vertexPositionBuffer.destroy();
    m_vertexColorBuffer.destroy();
    m_vertexTexCoordBuffer.destroy();
    m_vertexNormalBuffer.destroy();
    m_vertexTangentBuffer.destroy();
    m_indexBuffer.destroy();

    m_vao->destroy();
}

vector< shared_ptr<ModelData> > ModelLoader::loadModel(const string& filename,
                                                       const QOpenGLShaderProgramPtr& shader)
{
    m_shader = shader;

    Assimp::Importer Importer;
    const aiScene* scene = Importer.ReadFile(filename.c_str(),
                                             aiProcessPreset_TargetRealtime_MaxQuality |
                                             aiProcess_FlipUVs);

    if(scene == nullptr)
    {
        qFatal(qPrintable(QObject::tr("Error parsing : %1 -> %2").arg(filename.c_str()).arg(Importer.GetErrorString())));
        exit(1);
    }
    else if(scene->HasTextures())
    {
        qFatal("Support for meshes with embedded textures is not implemented");
        exit(1);
    }

    vector<shared_ptr<ModelData>> modelData = vector<shared_ptr<ModelData>>();
    modelData.resize(scene->mNumMeshes);

    qDebug() << "Model has" << modelData.size() << "meshes";

    unsigned int numVertices = 0;
    unsigned int numIndices  = 0;

    for(unsigned int i = 0; i < modelData.size(); i++)
    {
        numVertices += scene->mMeshes[i]->mNumVertices;
        numIndices  += scene->mMeshes[i]->mNumFaces * 3;
    }

    m_positions.reserve(numVertices);
    m_colors.reserve(numVertices);
    m_normals.reserve(numVertices);
    m_texCoords.reserve(numVertices);
    m_tangents.reserve(numVertices);
    m_indices.reserve(numIndices);

    numVertices = 0;
    numIndices  = 0;

    for(unsigned int i = 0; i < modelData.size(); i++)
    {
        modelData[i] = make_shared<ModelData>();

        modelData[i]->meshData     = loadMesh(i, numVertices, numIndices, scene->mMeshes[i]);
        modelData[i]->textureData  = loadTexture(filename, scene->mMaterials[scene->mMeshes[i]->mMaterialIndex]);
        modelData[i]->materialData = loadMaterial(i, scene->mMaterials[scene->mMeshes[i]->mMaterialIndex]);

        numVertices += scene->mMeshes[i]->mNumVertices;
        numIndices  += scene->mMeshes[i]->mNumFaces * 3;
    }

    prepareVertexBuffers();

    qDebug() << "Model has" << numVertices << "vertices";

    return modelData;
}

MeshData ModelLoader::loadMesh(unsigned int index,
                               unsigned int numVertices,
                               unsigned int numIndices,
                               const aiMesh* mesh)
{
    MeshData data = MeshData();

    if(mesh->mName.length > 0)
        data.name = string(mesh->mName.C_Str());
    else
        data.name = "mesh_" + to_string(index);

    data.numIndices = mesh->mNumFaces * 3;
    data.baseVertex = numVertices;
    data.baseIndex  = numIndices;

    prepareVertexContainers(mesh);

    return data;
}

void ModelLoader::prepareVertexContainers(const aiMesh* mesh)
{
    const aiVector3D zero3D(0.0f, 0.0f, 0.0f);
    const aiColor4D  zeroColor(1.0f, 1.0f, 1.0f, 1.0f);

    // Populate the vertex attribute vectors
    for(unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        const aiVector3D * pPos      = &(mesh->mVertices[i]);
        const aiColor4D  * pColor    = mesh->HasVertexColors(0)         ? &(mesh->mColors[0][i])        : &zeroColor;
        const aiVector3D * pTexCoord = mesh->HasTextureCoords(0)        ? &(mesh->mTextureCoords[0][i]) : &zero3D;
        const aiVector3D * pNormal   = mesh->HasNormals()               ? &(mesh->mNormals[i])          : &zero3D;
        const aiVector3D * pTangent  = mesh->HasTangentsAndBitangents() ? &(mesh->mTangents[i])         : &zero3D;

        m_positions.push_back(QVector3D(pPos->x, pPos->y, pPos->z));
           m_colors.push_back(QVector4D(pColor->r, pColor->g, pColor->b, pColor->a));
        m_texCoords.push_back(QVector2D(pTexCoord->x, pTexCoord->y));
          m_normals.push_back(QVector3D(pNormal->x, pNormal->y, pNormal->z));
         m_tangents.push_back(QVector3D(pTangent->x, pTangent->y, pTangent->z));
    }

    // Populate the index buffer
    for(unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        const aiFace& face = mesh->mFaces[i];

        if(face.mNumIndices != 3)
        {
            // Unsupported modes : GL_POINTS / GL_LINES / GL_POLYGON
            qFatal("Unable to load model... Unsupported number of indices per face.");
            exit(1);
            break;
        }

        m_indices.push_back(face.mIndices[0]);
        m_indices.push_back(face.mIndices[1]);
        m_indices.push_back(face.mIndices[2]);
    }
}

void ModelLoader::prepareVertexBuffers()
{
    m_vao->create();
    m_vao->bind();

    #pragma GCC diagnostic ignored "-Wconversion"

    // Generate and populate the buffers with vertex attributes and the indices
    m_vertexPositionBuffer.create();
    m_vertexPositionBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_vertexPositionBuffer.bind();
    m_vertexPositionBuffer.allocate(m_positions.data(), m_positions.size() * sizeof(QVector3D));

    m_vertexColorBuffer.create();
    m_vertexColorBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_vertexColorBuffer.bind();
    m_vertexColorBuffer.allocate(m_colors.data(), m_colors.size() * sizeof(QVector4D));

    m_vertexTexCoordBuffer.create();
    m_vertexTexCoordBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_vertexTexCoordBuffer.bind();
    m_vertexTexCoordBuffer.allocate(m_texCoords.data(), m_texCoords.size() * sizeof(QVector2D));

    m_vertexNormalBuffer.create();
    m_vertexNormalBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_vertexNormalBuffer.bind();
    m_vertexNormalBuffer.allocate(m_normals.data(), m_normals.size() * sizeof(QVector3D));

    m_vertexTangentBuffer.create();
    m_vertexTangentBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_vertexTangentBuffer.bind();
    m_vertexTangentBuffer.allocate(m_tangents.data(), m_tangents.size() * sizeof(QVector3D));

    m_indexBuffer.create();
    m_indexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_indexBuffer.bind();
    m_indexBuffer.allocate(m_indices.data(), m_indices.size() * sizeof(unsigned int));

    #pragma GCC diagnostic pop

    m_shader->bind();

    m_vertexPositionBuffer.bind();
    m_shader->enableAttributeArray("position");
    m_shader->setAttributeBuffer("position", GL_FLOAT, 0, 3);

    m_vertexColorBuffer.bind();
    m_shader->enableAttributeArray("color");
    m_shader->setAttributeBuffer("color", GL_FLOAT, 0, 4);

    m_vertexTexCoordBuffer.bind();
    m_shader->enableAttributeArray("texCoord");
    m_shader->setAttributeBuffer("texCoord", GL_FLOAT, 0, 2);

    m_vertexNormalBuffer.bind();
    m_shader->enableAttributeArray("normal");
    m_shader->setAttributeBuffer("normal", GL_FLOAT, 0, 3);

    m_vertexTangentBuffer.bind();
    m_shader->enableAttributeArray("tangent");
    m_shader->setAttributeBuffer("tangent", GL_FLOAT, 0, 3);

    m_vao->release();
}

MaterialData ModelLoader::loadMaterial(unsigned int index,
                                       const aiMaterial* material)
{
    Q_ASSERT(material != nullptr);

    MaterialData data = MaterialData();
    data.name = "material_" + to_string(index);

    aiColor3D ambientColor(0.1f, 0.1f, 0.1f);

    if(material->Get(AI_MATKEY_COLOR_AMBIENT, ambientColor) == AI_SUCCESS)
    {
        data.ambientColor.setX(ambientColor.r);
        data.ambientColor.setY(ambientColor.g);
        data.ambientColor.setZ(ambientColor.b);
        data.ambientColor.setW(1.0f);
    }

    aiColor3D diffuseColor(1.0f, 1.0f, 1.0f);

    if(material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor) == AI_SUCCESS)
    {
        data.diffuseColor.setX(diffuseColor.r);
        data.diffuseColor.setY(diffuseColor.g);
        data.diffuseColor.setZ(diffuseColor.b);
        data.diffuseColor.setW(1.0f);
    }

    aiColor3D specularColor(0.7f, 0.7f, 0.7f);

    if(material->Get(AI_MATKEY_COLOR_SPECULAR, specularColor) == AI_SUCCESS)
    {
        data.specularColor.setX(specularColor.r);
        data.specularColor.setY(specularColor.g);
        data.specularColor.setZ(specularColor.b);
        data.specularColor.setW(1.0f);
    }

    aiColor3D emissiveColor(0.0f, 0.0f, 0.0f);

    if(material->Get(AI_MATKEY_COLOR_EMISSIVE, emissiveColor) == AI_SUCCESS)
    {
        data.emissiveColor.setX(emissiveColor.r);
        data.emissiveColor.setY(emissiveColor.g);
        data.emissiveColor.setZ(emissiveColor.b);
        data.emissiveColor.setW(1.0f);
    }

    float shininess = 64.0f;

    if(material->Get(AI_MATKEY_SHININESS, shininess) == AI_SUCCESS)
    {
        data.shininess = shininess;
    }

    float shininessStrength = 1.0f;

    if(material->Get(AI_MATKEY_SHININESS_STRENGTH, shininessStrength) == AI_SUCCESS)
    {
        data.shininessStrength = shininessStrength;
    }

    return data;
}

TextureData ModelLoader::loadTexture(const string& filename,
                                     const aiMaterial* material)
{
    Q_ASSERT(material != nullptr);

    string dir;
    string::size_type slashIndex = filename.find_last_of("/");

    if(slashIndex == string::npos) dir = ".";
    else if(slashIndex == 0) dir = "/";
    else dir = filename.substr(0, slashIndex);

    TextureData data = TextureData();
    aiString path;

    if(material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
    {
        if(material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
        {
            string texturePath = dir + "/" + path.data;
            data.filename = texturePath;
        }
    }
    else
    {
        data.filename = "assets/white.png";
    }

    return data;
}

QOpenGLVertexArrayObjectPtr ModelLoader::getVAO()
{
    return m_vao;
}
