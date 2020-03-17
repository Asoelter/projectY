#include <Windows.h>

#include <cassert>

#include <gui/window.h>
#include <util/uuid.h>

#include <d3d11.h>

int WINAPI WinMain(_In_     HINSTANCE hInstance, 
                   _In_opt_ HINSTANCE hPrevInstance,
                   _In_     PSTR szCmdLine, 
                   _In_     int iCmdShow)
{
    auto window = gui::Window(1000, 750, "Main window");

    gui::Button::Descriptor desc;
    desc.x = 10;
    desc.y = 20;
    desc.width = 100;
    desc.height = 20;
    desc.title = "Hide Cursor";

    auto button = gui::Button(desc);

    BOOL showCursor = FALSE;

    button.pushed.connect([&showCursor]() {ShowCursor(showCursor); showCursor = !showCursor; });

    window.attach(std::move(button));

    DXGI_MODE_DESC modeDescription;

    modeDescription.Width = static_cast<UINT>(window.width());
    modeDescription.Height = static_cast<UINT>(window.height());
    modeDescription.RefreshRate = { 60, 1 }; //60 frames per second
    modeDescription.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    modeDescription.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    modeDescription.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

    DXGI_SAMPLE_DESC sampleDescription;

    //default quality
    sampleDescription.Count = 1;
    sampleDescription.Quality = 0; 

    DXGI_SWAP_CHAIN_DESC swapDescription;

    swapDescription.BufferDesc = modeDescription;
    swapDescription.SampleDesc = sampleDescription;
    swapDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; //this is for screen output
    swapDescription.BufferCount = 1; //one back buffer
    swapDescription.OutputWindow = window.handle();
    swapDescription.Windowed = true;
    swapDescription.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; //discard back buffer after use
    swapDescription.Flags = 0;

    //TODO(asoelter): move to ComPtr's for pointer types
    D3D_FEATURE_LEVEL featureLevels[] = {D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0};
    ID3D11Device* device;
    D3D_FEATURE_LEVEL selectedFeatureLevel;
    ID3D11DeviceContext* context;
    IDXGISwapChain* swapchain;

    auto flags = static_cast<UINT>(D3D11_CREATE_DEVICE_SINGLETHREADED);

#ifdef DEBUG
    flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    auto result = D3D11CreateDeviceAndSwapChain(
        nullptr, 
        D3D_DRIVER_TYPE_HARDWARE, 
        nullptr, 
        flags,  
        featureLevels, 
        2, 
        D3D11_SDK_VERSION, 
        &swapDescription, 
        &swapchain, 
        &device,
        &selectedFeatureLevel,
        &context
    );

    if (FAILED(result))
    {
        MessageBox(NULL, "unknown error", "unknown error", 0);
    }

    //This basically allows Direct3D to 
    //draw to the backbuffer
    ID3D11RenderTargetView* renderView;
    ID3D11Texture2D* backBuffer;
    swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D),reinterpret_cast<void**>(&backBuffer));

    if (!backBuffer)
    {
        MessageBox(NULL, "unable to make backbuffer", "unable to make backbuffer", 0);
        return 0;
    }

    device->CreateRenderTargetView(backBuffer, nullptr, &renderView);
    backBuffer->Release();

    D3D11_TEXTURE2D_DESC depthBufferDesc;

    depthBufferDesc.Width = window.width();
    depthBufferDesc.Height = window.height();
    depthBufferDesc.MipLevels = 1;
    depthBufferDesc.ArraySize = 1;
    depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthBufferDesc.SampleDesc = sampleDescription;
    depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthBufferDesc.CPUAccessFlags = NULL;
    depthBufferDesc.MiscFlags = NULL;

    ID3D11Texture2D* depthBuffer;
    ID3D11DepthStencilView* depthView;

    device->CreateTexture2D(
        &depthBufferDesc, // Description of texture to create.
        nullptr,
        &depthBuffer); // Return pointer to depth/stencil buffer.

    assert(depthBuffer);

    device->CreateDepthStencilView(
        depthBuffer, // Resource we want to create a view to.
        nullptr,
        &depthView); // Return depth/stencil view
    context->OMSetRenderTargets(1, &renderView, depthView);

    D3D11_VIEWPORT viewport;

    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;
    viewport.Width    = window.width();
    viewport.Height   = window.height();
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;

    context->RSSetViewports(1, &viewport);

    while (window.open())
    {
        window.update();
    }

    return 0;
}

