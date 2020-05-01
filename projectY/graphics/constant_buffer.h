#ifndef CONSTANT_BUFFER_H
#define CONSTANT_BUFFER_H

#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl.h>

#include "color.h"

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

    void bind(ID3D11Device* device, ID3D11DeviceContext* context);

    const void* rawBuffer() const;
    const D3D11_BUFFER_DESC& description() const;
    const D3D11_SUBRESOURCE_DATA& data() const;
private:
    Microsoft::WRL::ComPtr<ID3D11Buffer> buffer_;
    D3D11_BUFFER_DESC description_;
    D3D11_SUBRESOURCE_DATA resourceData_;
    BufferType type_;
};

#include "constant_buffer.hpp"

struct MatrixBuffer
{
    DirectX::XMMATRIX transform;
};

struct ColorBuffer
{
    ColorBuffer(const Color& c) : color(c) {}
    Color color;
};


#endif //CONSTANT_BUFFER_H