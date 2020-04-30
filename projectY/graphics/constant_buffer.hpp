#include <stdexcept>
#include "constant_buffer.h"

template<typename T>
ConstantBuffer<T>::ConstantBuffer(const T& data)
    : buffer_(nullptr)
    , description_{0}
    , resourceData_{0}
{
    description_.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    description_.Usage = D3D11_USAGE_DYNAMIC;
    description_.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    description_.MiscFlags = 0u;
    description_.ByteWidth = sizeof(T);
    description_.StructureByteStride = 0u;
    resourceData_.pSysMem = &data;
}

template<typename T>
void ConstantBuffer<T>::bind(ID3D11Device* device, ID3D11DeviceContext* context)
{
    if (!device)
    {
        throw std::invalid_argument("Constant buffer bound to renderer with invalid device");
    }
    if (!context)
    {
        throw std::invalid_argument("Constant buffer bound to renderer with invalid device context");
    }

    if (!buffer_)
    {
        device->CreateBuffer(&description_, &resourceData_, buffer_.GetAddressOf());
    }

    context->VSSetConstantBuffers(slotCounter.slotNumber++, 1u, buffer_.GetAddressOf());
}

template<typename T>
inline void ConstantBuffer<T>::clearBuffers()
{
    slotCounter.slotNumber = 0;
}

template<typename T>
const void* ConstantBuffer<T>::rawBuffer() const 
{ 
    return buffer_.GetAddressOf(); 
}

template<typename T>
const D3D11_BUFFER_DESC& ConstantBuffer<T>::description() const 
{
    return description_;
}

template<typename T>
const D3D11_SUBRESOURCE_DATA& ConstantBuffer<T>::data() const 
{ 
    return resourceData_; 
}
