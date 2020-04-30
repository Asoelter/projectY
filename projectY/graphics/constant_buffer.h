#ifndef CONSTANT_BUFFER_H
#define CONSTANT_BUFFER_H

#include <d3d11.h>
#include <wrl.h>

namespace detail
{
struct SlotCounter
{
    inline static UINT slotNumber = 0;
};
}

template<typename T>
class ConstantBuffer
{
public:
    ConstantBuffer(const T& data);

    void bind(ID3D11Device* device, ID3D11DeviceContext* context);
    static void clearBuffers();

    const void* rawBuffer() const;
    const D3D11_BUFFER_DESC& description() const;
    const D3D11_SUBRESOURCE_DATA& data() const;
private:
    Microsoft::WRL::ComPtr<ID3D11Buffer> buffer_;
    D3D11_BUFFER_DESC description_;
    D3D11_SUBRESOURCE_DATA resourceData_;
    static detail::SlotCounter slotCounter;
};

#include "constant_buffer.hpp"

#endif //CONSTANT_BUFFER_H