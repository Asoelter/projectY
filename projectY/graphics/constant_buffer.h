#ifndef CONSTANT_BUFFER_H
#define CONSTANT_BUFFER_H

#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl.h>

#include "color.h"

template<int>
struct SlotCounter
{
    inline static UINT slotNumber = 0;
};

enum class BufferType
{
    Vertex,
    Pixel
};

template<typename T>
class ConstantBuffer
{
public:
    ConstantBuffer(const T& data, BufferType type);

    ConstantBuffer& operator=(const ConstantBuffer& rhs);

    void bind(ID3D11Device* device, ID3D11DeviceContext* context);
    static void clearBuffers();

    const void* rawBuffer() const;
    const D3D11_BUFFER_DESC& description() const;
    const D3D11_SUBRESOURCE_DATA& data() const;

private:
    static SlotCounter<0> vertexSlot;
    static SlotCounter<1> pixelSlot;
private:
    Microsoft::WRL::ComPtr<ID3D11Buffer> buffer_;
    D3D11_BUFFER_DESC description_;
    D3D11_SUBRESOURCE_DATA resourceData_;
    BufferType type_;
    T value_;
};

#include "constant_buffer.hpp"

struct MatrixBuffer
{
    MatrixBuffer(const DirectX::XMMATRIX& t) : transform(t) {}
    DirectX::XMMATRIX transform;
};

struct ColorBuffer
{
    ColorBuffer(const Color& c) : color(c) {}
    Color color;
};


#endif //CONSTANT_BUFFER_H