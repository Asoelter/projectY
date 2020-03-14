#include <Windows.h>

#include <cassert>

#include <gui/window.h>

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

    modeDescription.Width = window.width();
    modeDescription.Height = window.height();
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

    ID3D11RenderTargetView* view;
    ID3D11Texture2D* backBuffer;
    swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D),reinterpret_cast<void**>(&backBuffer));

    if (!backBuffer)
    {
        MessageBox(NULL, "unable to make backbuffer", "unable to make backbuffer", 0);
        return 0;
    }

    device->CreateRenderTargetView(backBuffer, nullptr, &view);
    backBuffer->Release();

    while (window.open())
    {
        window.update();
        swapchain->Present(1u, 0);
    }

    return 0;
}

