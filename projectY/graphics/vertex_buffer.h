#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include <d3d11.h>

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

    void bind() const;
    void unbind() const;
private:
    std::vector<float>             data_;
    std::vector<ElementDescriptor> descriptors_;
};

#endif //VERTEX_BUFFER_H