#include "modelmanager.h"
#include "scene.h"
#include "model.h"
#include "abstractmodel.h"

ModelManager::ModelManager(Scene* scene)
    : m_scene(scene),
      m_modelLoader(ModelLoader())
{}

ModelManager::~ModelManager() {}

AbstractModel* ModelManager::getModel(const string& name)
{
    if(m_models.find(name) != m_models.end())
    {
        return m_models[name].get();
    }

    return nullptr;
}

shared_ptr<Model> ModelManager::loadModel(const string& name,
                                          const string& filename,
                                          const QOpenGLShaderProgramPtr& shader)
{
    vector<shared_ptr<ModelData>> modelData = m_modelLoader.loadModel(filename, shader);

    m_models[name] = make_shared<Model>(m_scene,
                                        m_modelLoader.getVAO(),
                                        modelData);

    return m_models[name];
}
