#include "vertex_buffer.h"

#include <algorithm>

template<typename Vertex>
VertexBuffer<Vertex>::VertexBuffer()
    : vertices_()
    , buffer_(nullptr)
{
    for (size_t i = 0; i < VertexInfo<Vertex>::elementCount; ++i)
    {
        bufferInfo_[i].bitOrder = VertexInfo<Vertex>::bitOrders[i];
        bufferInfo_[i].semanticName = VertexInfo<Vertex>::semanticNames[i];
    }
}

template<typename Vertex>
VertexBuffer<Vertex>::VertexBuffer(std::vector<Vertex> vertices)
    : vertices_(vertices)
    , buffer_()
{
    for (size_t i = 0; i < VertexInfo<Vertex>::elementCount; ++i)
    {
        bufferInfo_[i].bitOrder = VertexInfo<Vertex>::bitOrders[i];
        bufferInfo_[i].semanticName = VertexInfo<Vertex>::semanticNames[i];
    }
}

template<typename Vertex>
void VertexBuffer<Vertex>::bind(ID3D11Device* device, ID3D11DeviceContext* context)
{
    if (!buffer_)
    {
        createBuffer(device);
    }

    const UINT stride = sizeof(Vertex);
    const UINT offset = 0u;
    context->IASetVertexBuffers(0u, 1u, buffer_.GetAddressOf(), &stride, &offset);
}

template<typename Vertex>
void VertexBuffer<Vertex>::addVertex(const Vertex& vertex)
{
    if (buffer_)
    {
        //clear the buffer so it will be 
        //recreated next time it's bound
        buffer_ = nullptr;
    }

    vertices_.push_back(vertex);
}

template<typename Vertex>
void VertexBuffer<Vertex>::addVertices(const std::vector<Vertex>& vertices)
{
    vertices_.insert(vertices_.end(), vertices.begin(), vertices.end());
}

template<typename Vertex>
std::vector<D3D11_INPUT_ELEMENT_DESC> VertexBuffer<Vertex>::layout() const
{
    auto rval = std::vector<D3D11_INPUT_ELEMENT_DESC>();

    for (int i = 0; i < VertexInfo<Vertex>::elementCount; ++i)
    {
        D3D11_INPUT_ELEMENT_DESC desc;
        desc.SemanticName = VertexInfo<Vertex>::semanticNames[i];
        desc.SemanticIndex = 0;
        desc.Format = static_cast<DXGI_FORMAT>(VertexInfo<Vertex>::bitOrders[i]);
        desc.InputSlot = 0;
        desc.AlignedByteOffset = VertexInfo<Vertex>::offsets[i];
        desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
        desc.InstanceDataStepRate = 0;

        rval.push_back(desc);
    }

    return rval;
}

template<typename Vertex>
size_t VertexBuffer<Vertex>::size() const
{
    return vertices_.size();
}

template<typename Vertex>
void VertexBuffer<Vertex>::createBuffer(ID3D11Device* device)
{
    D3D11_BUFFER_DESC bd = {0};
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.CPUAccessFlags = 0u;
    bd.MiscFlags = 0u;
    bd.ByteWidth = static_cast<UINT>(sizeof(vertices_[0]) * vertices_.size());
    bd.StructureByteStride = sizeof(Vertex);

    D3D11_SUBRESOURCE_DATA srd = {0};
    auto* rawData = vertices_.data();
    srd.pSysMem = (void*)rawData;

    device->CreateBuffer(&bd, &srd, &buffer_);
}
