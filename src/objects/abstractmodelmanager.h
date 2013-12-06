#ifndef ABSTRACTMODELMANAGER_H
#define ABSTRACTMODELMANAGER_H

#include <string>
#include <memory>

#include "modelloader.h"

using namespace std;

class AbstractModel;
class Model;

class AbstractModelManager
{

public:
    AbstractModelManager();
    virtual ~AbstractModelManager() = 0;

    virtual AbstractModel* getModel(const string& name) = 0;

    virtual shared_ptr<Model> loadModel(const string& name,
                                        const string& filename,
                                        const QOpenGLShaderProgramPtr& shader) = 0;
};

#endif // ABSTRACTMODELMANAGER_H
