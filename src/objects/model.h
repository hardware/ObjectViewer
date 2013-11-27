#ifndef MODEL_H
#define MODEL_H

#include "abstractmodel.h"

#include <vector>
#include <memory>

using namespace std;

class Scene;
class Mesh;
class Texture;
class Material;
class AbstractMeshManager;
class AbstractMaterialManager;
class AbstractTextureManager;

class Model : public AbstractModel
{

public:
    Model(Scene* scene);
    Model(Scene* scene, vector<shared_ptr<ModelData>> modelData);
    virtual ~Model();

    virtual void render(const QOpenGLShaderProgramPtr& shader);

protected:
    Scene* m_scene;

    vector<Mesh*>     m_meshes;
    vector<Texture*>  m_textures;
    vector<Material*> m_materials;

    AbstractMeshManager     * m_meshManager;
    AbstractMaterialManager * m_materialManager;
    AbstractTextureManager  * m_textureManager;

private:
    void initialize(vector<shared_ptr<ModelData>> modelData = vector<shared_ptr<ModelData>>());
    void destroy();

};

#endif // MODEL_H
