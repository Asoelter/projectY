#ifndef MESH_H
#define MESH_H

#include "constant_buffer.h"
#include "vertex_buffer.h"

class Mesh
{
public:
    Mesh();

private:
    VertexBuffer<PVertex> vertexBuffer_;
    ConstantBuffer<ColorBuffer> color_;
};

#endif //MESH_H