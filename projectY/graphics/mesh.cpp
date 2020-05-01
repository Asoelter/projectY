#include "mesh.h"

#include "renderer.h"

Mesh::Mesh(const std::vector<PVertex>& vertices, const Color& color, DrawMode mode)
    : vertexBuffer_(vertices)
    , color_(color)
    , colorBuffer_(color_)
    , constColorBuffer_(colorBuffer_, BufferType::Pixel)
    , drawMode_(mode)
{
}

void Mesh::addVertex(const PVertex& vertex)
{
    vertexBuffer_.addVertex(vertex);
}

void Mesh::addVertices(const std::vector<PVertex>& vertices)
{
    vertexBuffer_.addVertices(vertices);
}

void Mesh::setDrawMode(DrawMode mode)
{
    drawMode_ = mode;
}

DrawMode Mesh::drawMode() const
{
    return drawMode_;
}

Color Mesh::color() const
{
    return color_;
}

void Mesh::setColor(const Color& color)
{
    color_ = color;
    colorBuffer_ = ColorBuffer(color_);
    constColorBuffer_ = ConstantBuffer<ColorBuffer>(colorBuffer_, BufferType::Pixel);
}

void Mesh::draw(Renderer& renderer)
{
    renderer.bindBuffer(vertexBuffer_);
    renderer.bindConstantBuffer(constColorBuffer_);
    renderer.draw(drawMode_);
}
