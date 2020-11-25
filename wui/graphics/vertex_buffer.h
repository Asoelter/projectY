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

    VertexBuffer();
    VertexBuffer(std::vector<Vertex> vertices);

    void bind(ID3D11Device* device, ID3D11DeviceContext* context);
    void addVertex(const Vertex& vertex);
    void addVertices(const std::vector<Vertex>& vertices);

    [[nodiscard]]
    std::vector<D3D11_INPUT_ELEMENT_DESC> layout() const;

    [[nodiscard]]
    size_t size() const;


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

struct PVertex
{
    DirectX::XMFLOAT4 position;
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
    static constexpr char* const semanticNames[elementCount] = { "Position", "Color" };
    static constexpr UINT offsets[elementCount] = { 0, 16u };
};

template<>
struct VertexInfo<PVertex>
{
    static constexpr auto elementCount = 1;
    static constexpr VertexBuffer<PVertex>::BitOrder bitOrders[elementCount] = { VertexBuffer<PVertex>::BitOrder::X32Y32Z32W32};
    static constexpr char* const semanticNames[elementCount] = { "Position" };
    static constexpr UINT offsets[elementCount] = { 0u };
};

#include "vertex_buffer.hpp"

#endif //VERTEX_BUFFER_H