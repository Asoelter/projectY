#ifndef RENDERER_H
#define RENDERER_H

#include <d3d11.h>
#include <wrl.h>

#include <gui/window.h>

#include "color.h"
#include "vertex_buffer.h"
#include "vertex_shader.h"

enum class Topology
{
    TriangleList = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
};

class Renderer
{
public:
    Renderer(const gui::Window& window);

    template<typename Shader>
    void bind(Shader& shader);

    template<typename Vertex>
    void bind(VertexBuffer<Vertex>& buffer);

    void clear(const Color& color);
    void draw(Topology topology);

    //temp functions to rough in the renderer
    Microsoft::WRL::ComPtr<ID3D11Device>           device()    { return device_; }
    Microsoft::WRL::ComPtr<IDXGISwapChain>         swapchain() { return swapchain_; }
    Microsoft::WRL::ComPtr<ID3D11DeviceContext>    context()   { return context_; }
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> target()    { return target_; }
    Microsoft::WRL::ComPtr<ID3D11VertexShader>     shader() { return shader_->rawShader(); }

private:
    template<typename Vertex, typename Shader>
    void registerBufferLayout(VertexBuffer<Vertex>& buffer, Shader& shader);
private:
    Microsoft::WRL::ComPtr<ID3D11Device>            device_;
    Microsoft::WRL::ComPtr<IDXGISwapChain>          swapchain_;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext>     context_;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  target_;
    VertexShader* shader_;
    size_t vertexCount_;
};

template<typename Shader>
void Renderer::bind(Shader& shader)
{
    shader_ = &shader;
    shader.bind(device_.Get(), context_.Get());
}

template<typename Vertex>
void Renderer::bind(VertexBuffer<Vertex>& buffer)
{
    if (!shader_)
    {
        assert(false && "shader must be bound before vertex buffer");
        MessageBox(NULL, "Hello", "Hello", 0);
        return;
    }
    buffer.bind(device_.Get(), context_.Get());
    vertexCount_ += buffer.size();

    registerBufferLayout(buffer, *shader_);
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