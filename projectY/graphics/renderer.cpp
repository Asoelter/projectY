#include "renderer.h"

D3D11_PRIMITIVE_TOPOLOGY translate(Topology topology)
{
    return static_cast<D3D11_PRIMITIVE_TOPOLOGY>(topology);
}

Renderer::Renderer(const gui::Window& window)
    : device_()
    , swapchain_()
    , context_()
    , target_()
    , shader_(nullptr)
    , vertexCount_(0u)
{
    DXGI_SWAP_CHAIN_DESC sd = {};
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 0;
    sd.BufferDesc.RefreshRate.Denominator = 0;
    sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.BufferCount = 1;
    sd.OutputWindow = window.handle();
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    sd.Flags = 0;

    UINT swapCreateFlags = 0u;
#ifdef DEBUG
    swapCreateFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    // create device and front/back buffers, and swap chain and rendering context
    D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        swapCreateFlags,
        nullptr,
        0,
        D3D11_SDK_VERSION,
        &sd,
        &swapchain_,
        &device_,
        nullptr,
        &context_
    );
    // gain access to texture subresource in swap chain (back buffer)
    Microsoft::WRL::ComPtr<ID3D11Resource> pBackBuffer;
    swapchain_->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer);
    device_->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &target_);

    D3D11_VIEWPORT vp;
    vp.Width = static_cast<FLOAT>(window.width());
    vp.Height = static_cast<FLOAT>(window.height());
    vp.MinDepth = 0;
    vp.MaxDepth = 1;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    context_->RSSetViewports(1u, &vp);
}

void Renderer::bindVertexShader(VertexShader& shader)
{
    shader_ = &shader;
    shader.bind(device_.Get(), context_.Get());
}

void Renderer::bindPixelShader(PixelShader& shader)
{
    shader.bind(device_.Get(), context_.Get());
}

void Renderer::beginFrame(const Color& color)
{
    context_->ClearRenderTargetView(target_.Get(), color.data);
}

void Renderer::draw(Topology topology)
{
    // bind render target
    context_->OMSetRenderTargets(1u, target_.GetAddressOf(), nullptr);

    // Set primitive topology to triangle list (groups of 3 vertices)
    D3D11_PRIMITIVE_TOPOLOGY d3dtopology = translate(topology);

    context_->IASetPrimitiveTopology(d3dtopology);
    context_->Draw((UINT)vertexCount_, 0u);

    //endFrame
    vertexCount_ = 0;
}

void Renderer::endFrame()
{
    swapchain_->Present(1u, 0u);
    //The type of the constant buffer below does
    //not matter because the clearBuffers method
    //works on data that is accessible to all
    //constant buffers of any kind
    ConstantBuffer<int>::clearBuffers(); 
}

