#ifndef MESH_H
#define MESH_H

#include <string>

#include <QVector>
#include <QVector3D>

#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QSharedPointer>

using namespace std;

typedef QSharedPointer<QOpenGLShaderProgram> QOpenGLShaderProgramPtr;

class QOpenGLFunctions_4_3_Core;
class QOpenGLVertexArrayObject;

class Mesh
{

public:
    Mesh(const string& name,
         const vector<QVector3D>& positions,
         const vector<QVector4D>& colors,
         const vector<QVector2D>& texCoords,
         const vector<QVector3D>& normals,
         const vector<QVector3D>& tangents,
         const QOpenGLShaderProgramPtr& shader);

    virtual ~Mesh();

    void render();

    void setName(const string& name) { m_name = name; }
    string name() const { return m_name; }

private:
    void init();
    void load();

    QOpenGLFunctions_4_3_Core * m_funcs;
    QOpenGLVertexArrayObject  * m_vao;

    QOpenGLBuffer m_vertexPositionBuffer;
    QOpenGLBuffer m_vertexColorBuffer;
    QOpenGLBuffer m_vertexTexCoordBuffer;
    QOpenGLBuffer m_vertexNormalBuffer;
    QOpenGLBuffer m_vertexTangentBuffer;

    string m_name;

    vector<QVector3D> m_positions;
    vector<QVector4D> m_colors;
    vector<QVector2D> m_texCoords;
    vector<QVector3D> m_normals;
    vector<QVector3D> m_tangents;

    QOpenGLShaderProgramPtr m_shader;

};

#endif // MESH_H
