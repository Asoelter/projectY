#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include <DirectXMath.h>
#include <d3d11.h>
#include <wrl.h>

#include <string>
#include <vector>

template<typename T>
struct VertexInfo
{
    VertexInfo() { assert(false && "You must overload the VertexInfo struct"); }
};

template<typename Vertex>
class VertexBuffer
{
public:
    enum class BitOrder
    {
        R32G32       = DXGI_FORMAT_R32G32_FLOAT,
        R32G32B32    = DXGI_FORMAT_R32G32B32_FLOAT,
        R32G32B32A32 = DXGI_FORMAT_R32G32B32A32_FLOAT,
        X32Y32       = R32G32,
        X32Y32Z32    = R32G32B32,
        X32Y32Z32W32 = R32G32B32A32
    };

    VertexBuffer(std::vector<Vertex> vertices);

    void bind(ID3D11Device* device, ID3D11DeviceContext* context);

    //temp function to rough the vertex buffer in with
    Microsoft::WRL::ComPtr<ID3D11Buffer> buffer() { return buffer_; }

private:
    void createBuffer(ID3D11Device* device);

private:
    struct BufferInfo
    {
        BitOrder bitOrder;
        std::string semanticName;
    };

    std::vector<Vertex> vertices_;
    Microsoft::WRL::ComPtr<ID3D11Buffer> buffer_;
    BufferInfo bufferInfo_[VertexInfo<Vertex>::elementCount];
};

struct PCVertex
{
    DirectX::XMFLOAT4 position;
    DirectX::XMFLOAT4 color;
};

template<>
struct VertexInfo<PCVertex>
{
    static constexpr auto elementCount = 2;
    static constexpr VertexBuffer<PCVertex>::BitOrder bitOrders[elementCount] = 
        { VertexBuffer<PCVertex>::BitOrder::X32Y32Z32W32, VertexBuffer<PCVertex>::BitOrder::R32G32B32A32 };
    static constexpr char* semanticNames[elementCount] = { "Position", "Color" };
};

#include "vertex_buffer.hpp"

#endif //VERTEX_BUFFER_H