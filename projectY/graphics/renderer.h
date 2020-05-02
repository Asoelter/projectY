#ifndef RENDERER_H
#define RENDERER_H

#include <d3d11.h>
#include <wrl.h>

#include <gui/window.h>

#include "color.h"
#include "constant_buffer.h"
#include "draw_mode.h"
#include "pixel_shader.h"
#include "vertex_buffer.h"
#include "vertex_shader.h"

class Mesh;

class Renderer
{
public:
    Renderer(const gui::Window& window, const DirectX::XMMATRIX& projection = DirectX::XMMatrixIdentity());

    void bindVertexShader(VertexShader& shader);
    void bindPixelShader(PixelShader& shader);

    template<typename Vertex>
    void bindBuffer(VertexBuffer<Vertex>& buffer);

    template<typename BufferType>
    void bindConstantBuffer(ConstantBuffer<BufferType>& constBuffer);

    void beginFrame(const Color& color);
    void draw(DrawMode mode);
    void draw(Mesh& mesh);
    void endFrame();

private:
    template<typename Vertex, typename Shader>
    void registerBufferLayout(VertexBuffer<Vertex>& buffer, Shader& shader);
private:
    Microsoft::WRL::ComPtr<ID3D11Device>            device_;
    Microsoft::WRL::ComPtr<IDXGISwapChain>          swapchain_;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext>     context_;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  target_;
    DirectX::XMMATRIX                               projection_;
    ConstantBuffer<MatrixBuffer>                    projectionBuffer_;
    VertexShader*                                   shader_;
    size_t                                          vertexCount_;
};

template<typename Vertex>
void Renderer::bindBuffer(VertexBuffer<Vertex>& buffer)
{
    if (!shader_)
    {
        MessageBox(NULL, "Shader Error", "Shader must be bound before vertex buffer", 0);
        assert(false && "shader must be bound before vertex buffer");
        return;
    }
    buffer.bind(device_.Get(), context_.Get());
    vertexCount_ += buffer.size();

    registerBufferLayout(buffer, *shader_);
}

template<typename BufferType>
void Renderer::bindConstantBuffer(ConstantBuffer<BufferType>& constBuffer)
{
    constBuffer.bind(device_.Get(), context_.Get());
}

template<typename Vertex, typename Shader>
void Renderer::registerBufferLayout(VertexBuffer<Vertex>& buffer, Shader& shader)
{
    auto layout = buffer.layout();
    auto blob = shader_->blob();

    Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;
    device_->CreateInputLayout(
        layout.data(), (UINT)layout.size(),
        blob->GetBufferPointer(),
        blob->GetBufferSize(),
        &pInputLayout
    );

    context_->IASetInputLayout(pInputLayout.Get());
}

#endif //RENDERER_H