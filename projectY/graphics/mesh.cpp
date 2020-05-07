#include "mesh.h"

#include "renderer.h"

Mesh::Mesh(const std::vector<PVertex>& vertices, const Color& color, DrawMode mode)
    : vertexBuffer_(vertices)
    , color_(color)
    , colorBuffer_(ColorBuffer(color_), BufferType::Pixel)
    , modelMatrix_(DirectX::XMMatrixIdentity())
    , matrixBuffer_({modelMatrix_}, BufferType::Vertex)
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
    colorBuffer_ = ConstantBuffer<ColorBuffer>(ColorBuffer(color_), BufferType::Pixel);
}

void Mesh::draw(Renderer& renderer)
{
    renderer.bindBuffer(vertexBuffer_);
    renderer.bindConstantBuffer(colorBuffer_);
    renderer.bindConstantBuffer(matrixBuffer_);
    renderer.draw(drawMode_);
}

void Mesh::translate(float x, float y, float z)
{
    const auto translation = DirectX::XMMatrixTranslation(x, y, z);
    modelMatrix_ *= translation;
    matrixBuffer_ = ConstantBuffer<MatrixBuffer>(modelMatrix_, BufferType::Vertex);
}

void Mesh::rotate(float angle, const math::vec3<float>& axis)
{
    const auto rotation = DirectX::XMMatrixRotationAxis(axis.toXMVector(), angle);
    modelMatrix_ *= rotation;
    matrixBuffer_ = ConstantBuffer<MatrixBuffer>(modelMatrix_, BufferType::Vertex);
}
