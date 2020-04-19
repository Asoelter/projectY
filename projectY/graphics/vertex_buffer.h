#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include <d3d11.h>
#include <wrl.h>

#include <string>
#include <vector>

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

    struct ElementDescriptor 
    {
        std::string semanticName;
        BitOrder bitOrder;
    };

    VertexBuffer(const std::vector<float> data, const std::vector<ElementDescriptor>& desc);

    void bind(ID3D11DeviceContext* context) const;
    void unbind() const;

    bool hasRawbuffer() const noexcept;
    void createRawBuffer(ID3D11Device* device);
private:
    std::vector<float>             data_;
    std::vector<ElementDescriptor> descriptors_;
    Microsoft::WRL::ComPtr<ID3D11Buffer> buffer_;
};

#endif //VERTEX_BUFFER_H