#ifndef RENDERER_H
#define RENDERER_H

#include <d3d11.h>
#include <wrl.h>

#include <gui/window.h>

#include "vertex_buffer.h"

class Renderer
{
public:
    Renderer(const gui::Window& window);

    void bind(const VertexBuffer& buffer);

    //temp functions to rough in the renderer
    Microsoft::WRL::ComPtr<ID3D11Device>           device()    { return device_; }
    Microsoft::WRL::ComPtr<IDXGISwapChain>         swapchain() { return swapchain_; }
    Microsoft::WRL::ComPtr<ID3D11DeviceContext>    context()   { return context_; }
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> target()    { return target_; }
private:
    Microsoft::WRL::ComPtr<ID3D11Device>            device_;
    Microsoft::WRL::ComPtr<IDXGISwapChain>          swapchain_;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext>     context_;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  target_;
};

#endif //RENDERER_H