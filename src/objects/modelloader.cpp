#include "modelloader.h"

#include <QDebug>

ModelLoader::ModelLoader()
{
}

ModelLoader::~ModelLoader()
{

}

vector< shared_ptr<ModelData> > ModelLoader::loadModel(const string& filename)
{
    return loadModel(filename, filename);
}

vector< shared_ptr<ModelData> > ModelLoader::loadModel(const string& name, const string& filename)
{
    qDebug() << "Loading model : " << QString::fromStdString(name);

    Assimp::Importer Importer;
    const aiScene* scene = Importer.ReadFile(filename.c_str(),
                                             aiProcessPreset_TargetRealtime_MaxQuality ^
                                             aiProcess_JoinIdenticalVertices ^
                                             aiProcess_FindInvalidData
                                             );

    // aiProcess_FlipUVs

    if(scene == nullptr)
    {
        qFatal("Unable to load model...");
        exit(1);
    }
    else if(scene->HasTextures())
    {
        qFatal("Support for meshes with embedded textures is not implemented");
        exit(1);
    }

    vector<shared_ptr<ModelData>> modelData = vector<shared_ptr<ModelData>>();
    modelData.resize(scene->mNumMeshes);

    qDebug() << "Model has " << modelData.size() << " meshes";

    int numVertices = 0;

    for(unsigned int i = 0; i < modelData.size(); i++)
    {
        modelData[i] = std::shared_ptr<ModelData>(new ModelData());

        modelData[i]->meshData     =     loadMesh(name, filename, i, scene->mMeshes[i]);
        modelData[i]->materialData = loadMaterial(name, filename, i, scene->mMaterials[scene->mMeshes[i]->mMaterialIndex]);
        modelData[i]->textureData  =  loadTexture(name, filename, i, scene->mMaterials[scene->mMeshes[i]->mMaterialIndex]);

        numVertices += scene->mMeshes[i]->mNumVertices;
    }

    qDebug() << "Model has " << numVertices << " vertices";

    return modelData;
}

MeshData ModelLoader::loadMesh(const string& name,
                               const string& filename,
                               unsigned int index,
                               const aiMesh* mesh)
{
    Q_UNUSED(name);
    Q_UNUSED(filename);

    MeshData data = MeshData();

    if(mesh->mName.length > 0)
        data.name = string(mesh->mName.C_Str());
    else
        data.name = "mesh_" + to_string(index);

    qDebug() << "Mesh ->" << QString::fromStdString(data.name);

    unsigned int currentIndex = 0;

    vector<unsigned int> vertexIndexMap = vector<unsigned int>();
    vertexIndexMap.resize(mesh->mNumVertices);

    for(unsigned int i = 0; i < mesh->mNumFaces; ++i)
    {
        const aiFace& face = mesh->mFaces[i];

        switch(face.mNumIndices)
        {
        case 1:
            // Mode = GL_POINTS;
            qFatal("Unable to load model... Unsupported number of indices per face (1)");
            exit(1);
            break;

        case 2:
            // Mode = GL_LINES;
            qFatal("Unable to load model... Unsupported number of indices per face (2)");
            exit(1);
            break;

        case 4:
            // Mode = GL_POLYGON;
            qFatal("Unable to load model... Unsupported number of indices per face (4)");
            exit(1);
            break;
        }

        unsigned int numIndices = face.mNumIndices;

        data.positions.resize(currentIndex + numIndices);
        data.colors.resize(currentIndex + numIndices);
        data.texCoords.resize(currentIndex + numIndices);
        data.normals.resize(currentIndex + numIndices);
        data.tangents.resize(currentIndex + numIndices);

        for(unsigned int j = 0; j < numIndices; j++)
        {
            int vertexIndex = face.mIndices[j];
            vertexIndexMap[vertexIndex] = currentIndex + j;

            data.positions[currentIndex + j] = QVector3D(mesh->mVertices[vertexIndex].x,
                                                         mesh->mVertices[vertexIndex].y,
                                                         mesh->mVertices[vertexIndex].z);

            if(mesh->HasVertexColors(0))
            {
                data.colors[currentIndex + j] = QVector4D((float)mesh->mColors[0][vertexIndex].r,
                                                          (float)mesh->mColors[0][vertexIndex].g,
                                                          (float)mesh->mColors[0][vertexIndex].b,
                                                          (float)mesh->mColors[0][vertexIndex].a);
            }

            if(mesh->HasTextureCoords(0))
            {
                data.texCoords[currentIndex + j] = QVector2D(mesh->mTextureCoords[0][vertexIndex].x,
                                                             mesh->mTextureCoords[0][vertexIndex].y);
            }

            if(mesh->HasNormals())
            {
                data.normals[currentIndex + j] = QVector3D(mesh->mNormals[vertexIndex].x,
                                                           mesh->mNormals[vertexIndex].y,
                                                           mesh->mNormals[vertexIndex].z);
            }

            if(mesh->HasTangentsAndBitangents())
            {
                data.tangents[currentIndex + j] = QVector3D(mesh->mTangents[vertexIndex].x,
                                                            mesh->mTangents[vertexIndex].y,
                                                            mesh->mTangents[vertexIndex].z);
            }

        }

        currentIndex += 3;
    }

    return data;
}

MaterialData ModelLoader::loadMaterial(const string& name,
                                       const string& filename,
                                       unsigned int index,
                                       const aiMaterial* material)
{
    // assert( material != nullptr );
    Q_UNUSED(filename);

    MaterialData data = MaterialData();
    data.name = name + "_material_" + to_string(index);

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

    aiColor3D specularColor(0.0f, 0.0f, 0.0f);

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

    float shininess = 0.0f;

    if(material->Get(AI_MATKEY_SHININESS, shininess) == AI_SUCCESS)
    {
        data.shininess = shininess;
    }

    float shininessStrength = 1.0f;

    if(material->Get(AI_MATKEY_SHININESS_STRENGTH, shininessStrength) == AI_SUCCESS)
    {
        data.shininessStrength = shininessStrength;
    }

    qDebug() << "\t Done loading material : " << QString::fromStdString(data.name);

    return data;
}

TextureData ModelLoader::loadTexture(const string& name,
                                     const string& filename,
                                     unsigned int index,
                                     const aiMaterial* material)
{
    // Q_ASSERT(material == nullptr);

    Q_UNUSED(name);
    Q_UNUSED(filename);
    Q_UNUSED(index);

    TextureData data = TextureData();
    aiString path;

    if(material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
    {
        if(material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
        {
            qDebug() << "\t Done loading texture : " << QString::fromStdString(path.data);
            data.filename = path.data;
        }
    }
    else
    {
        qDebug() << "\t No texture for this mesh";
    }

    return data;
}
