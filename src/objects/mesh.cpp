#include "mesh.h"

Mesh::Mesh(const string& name,
           unsigned int numIndices,
           unsigned int baseVertex,
           unsigned int baseIndex)
    : m_name(name),
      m_numIndices(numIndices),
      m_baseVertex(baseVertex),
      m_baseIndex(baseIndex)
{}

Mesh::~Mesh() {}
