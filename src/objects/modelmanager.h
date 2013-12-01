#ifndef MODELMANAGER_H
#define MODELMANAGER_H

#include "abstractmodelmanager.h"
#include "modelloader.h"

class Scene;
class Model;
class AbstractModel;

class ModelManager : public AbstractModelManager
{

public:
    ModelManager(Scene* scene);
    virtual ~ModelManager();

    virtual AbstractModel* getModel(const string& name);
    virtual shared_ptr<Model> loadModel(const string& name, const string& filename);

private:
    map<string, shared_ptr<Model>> m_models;
    Scene* m_scene;
    ModelLoader m_modelLoader;
};

#endif // MODELMANAGER_H
