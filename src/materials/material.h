#ifndef MATERIAL_H
#define MATERIAL_H

#include <string>

#include <QVector4D>
#include <QOpenGLShaderProgram>
#include <QSharedPointer>

using namespace std;

typedef QSharedPointer<QOpenGLShaderProgram> QOpenGLShaderProgramPtr;

class QOpenGLFunctions_4_3_Core;

class Material
{

public:
    Material(const string& name,
             const QVector4D& ambientColor,
             const QVector4D& diffuseColor,
             const QVector4D& specularColor,
             const QVector4D& emissiveColor,
             float shininess,
             float shininessStrength,
             const QOpenGLShaderProgramPtr& shader);

    ~Material();

    void init();

    void setName(const string& name) { m_name = name; }
    string name() const { return m_name; }

private:
    void sendToGPU();

    QOpenGLFunctions_4_3_Core* m_funcs;

    string m_name;

    QVector4D m_ambientColor;
    QVector4D m_diffuseColor;
    QVector4D m_specularColor;
    QVector4D m_emissiveColor;

    float m_shininess;
    float m_shininessStrength;

    int m_twoSided;
    int m_twoSidedTrue;
    int m_wireframe;

    QOpenGLShaderProgramPtr m_shader;

};

#endif // MATERIAL_H
