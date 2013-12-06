#ifndef MESH_H
#define MESH_H

#include <string>

using namespace std;

class Mesh
{

public:
    Mesh(const string& name,
         unsigned int numIndices,
         unsigned int baseVertex,
         unsigned int baseIndex);

    virtual ~Mesh();

    void setName(const string& name) { m_name = name; }
    string name() const { return m_name; }

    unsigned int getNumIndices() const { return m_numIndices; }
    unsigned int getBaseVertex() const { return m_baseVertex; }
    unsigned int getBaseIndex()  const { return m_baseIndex;  }

private:
    string m_name;

    unsigned int m_numIndices;
    unsigned int m_baseVertex;
    unsigned int m_baseIndex;

};

#endif // MESH_H
