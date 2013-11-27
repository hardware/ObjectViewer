#include "modelmanager.h"

ModelManager::ModelManager(const QOpenGLShaderProgramPtr& shader)
    : m_shader(shader)
{}

ModelManager::~ModelManager() {}

AbstractModel* getModel(const string& name)
{
    Q_UNUSED(name);

    return nullptr;
}

void loadModel(const string& name, const string& filename)
{
    Q_UNUSED(name);
    Q_UNUSED(filename);
}

unique_ptr<AbstractModel> createModel(const string& name)
{
    Q_UNUSED(name);


}
