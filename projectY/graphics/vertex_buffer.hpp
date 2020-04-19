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

    assert(buffer_);
    const UINT stride = sizeof(Vertex);
    const UINT offset = 0u;
    context->IASetVertexBuffers(0u, 1u, buffer_.GetAddressOf(), &stride, &offset);
}

template<typename Vertex>
void VertexBuffer<Vertex>::createBuffer(ID3D11Device* device)
{
    Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
    D3D11_BUFFER_DESC bd = {0};
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(vertices_);
    bd.StructureByteStride = sizeof(Vertex);

    D3D11_SUBRESOURCE_DATA srd = {0};
    srd.pSysMem = vertices_.data();

    device->CreateBuffer(&bd, &srd, &buffer_);
}
