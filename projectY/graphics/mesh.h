#ifndef MESH_H
#define MESH_H

#include <vector>

#include "color.h"
#include "constant_buffer.h"
#include "draw_mode.h"
#include "vertex_buffer.h"

class Renderer;

class Mesh
{
public:
    Mesh(const std::vector<PVertex>& vertices = {}, 
         const Color& color = Color::Blue(), 
         DrawMode mode = DrawMode::TriangleList);

    void addVertex(const PVertex& vertex);
    void addVertices(const std::vector<PVertex>& vertices);

    void setDrawMode(DrawMode mode);
    [[nodiscard]] DrawMode drawMode() const;

    [[nodiscard]] Color color() const;
    void setColor(const Color& color);

    void draw(Renderer& renderer);

private:
    VertexBuffer<PVertex> vertexBuffer_;
    Color color_;
    ColorBuffer colorBuffer_;
    ConstantBuffer<ColorBuffer> constColorBuffer_;
    DrawMode drawMode_;
};

#endif //MESH_H