#include <Windows.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <wrl.h>

#include <cassert>

#include <gui/window.h>

#include <util/uuid.h>
#include <util/error_printer.h>

#include <d3d11.h>

struct Vertex
{
    DirectX::XMFLOAT4 position;
    DirectX::XMFLOAT3 color;
};

#define BREAK_ON_FAIL(res) assert(!FAILED(res));

int WINAPI WinMain(_In_     HINSTANCE hInstance, 
                   _In_opt_ HINSTANCE hPrevInstance,
                   _In_     PSTR szCmdLine, 
                   _In_     int iCmdShow)
{
#if defined(DEBUG) | defined(_DEBUG)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

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

    BREAK_ON_FAIL(result);

    //This basically allows Direct3D to 
    //draw to the backbuffer
    ID3D11RenderTargetView* renderView;
    Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
    result = swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), &backBuffer);
    BREAK_ON_FAIL(result);

    if (!backBuffer)
    {
        MessageBox(NULL, "unable to make backbuffer", "unable to make backbuffer", 0);
        return 0;
    }

    result = device->CreateRenderTargetView(backBuffer.Get(), nullptr, &renderView);
    BREAK_ON_FAIL(result);

    D3D11_TEXTURE2D_DESC depthBufferDesc;

    depthBufferDesc.Width = static_cast<UINT>(window.width());
    depthBufferDesc.Height = static_cast<UINT>(window.height());
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

    result = device->CreateTexture2D(
        &depthBufferDesc, // Description of texture to create.
        nullptr,
        &depthBuffer); // Return pointer to depth/stencil buffer.

    BREAK_ON_FAIL(result);
    assert(depthBuffer);

    result = device->CreateDepthStencilView(
        depthBuffer, // Resource we want to create a view to.
        nullptr,
        &depthView); // Return depth/stencil view

    BREAK_ON_FAIL(result);

    context->OMSetRenderTargets(1, &renderView, depthView);

    D3D11_VIEWPORT viewport;

    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;
    viewport.Width    = static_cast<UINT>(window.width());
    viewport.Height   = static_cast<UINT>(window.height());
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;

    context->RSSetViewports(1, &viewport);

    Vertex vertices[3] = {
        {DirectX::XMFLOAT4( 0.0f,  0.5f, 0.5f, 1.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f)},
        {DirectX::XMFLOAT4( 0.5f, -0.5f, 0.5f, 1.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f)},
        {DirectX::XMFLOAT4(-0.5f, -0.5f, 0.5f, 1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f)}
    };

    D3D11_INPUT_ELEMENT_DESC dataDescription[2] = {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };

    D3D11_BUFFER_DESC vbDesc = {};

    vbDesc.ByteWidth = sizeof(vertices);
    vbDesc.Usage = D3D11_USAGE_DEFAULT;
    vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbDesc.CPUAccessFlags = 0;
    vbDesc.MiscFlags = 0;
    vbDesc.StructureByteStride = sizeof(Vertex);

    D3D11_SUBRESOURCE_DATA srData = {};

    srData.pSysMem = vertices;

    Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
    result = device->CreateBuffer(&vbDesc, &srData, &vertexBuffer);
    BREAK_ON_FAIL(result);

    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    context->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);

    Microsoft::WRL::ComPtr<ID3DBlob> blob;
    Microsoft::WRL::ComPtr<ID3DBlob> errmsg;
    result = D3DCompileFromFile(
        L"../../../../projectY/res/shaders/pixel.hlsl",
        nullptr,
        nullptr,
        "main",
        "ps_5_0",
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
        0,
        &blob,
        &errmsg
    );

    BREAK_ON_FAIL(result);

    if (errmsg)
    {
        const char* msg = (const char *)errmsg->GetBufferPointer();
        MessageBox(NULL, msg, msg, 0);
    }

    Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
    result = device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &pixelShader);
    BREAK_ON_FAIL(result);

    result = D3DCompileFromFile(
        L"../../../../projectY/res/shaders/vertex.hlsl", 
        nullptr, 
        nullptr, 
        "main", 
        "vs_5_0", 
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 
        0, 
        &blob, 
        &errmsg);

    BREAK_ON_FAIL(result);

    if (errmsg)
    {
        const char* msg = (const char *)errmsg->GetBufferPointer();
        MessageBox(NULL, msg, msg, 0);
    }

    Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
    result = device->CreateInputLayout(dataDescription, 2, blob->GetBufferPointer(), blob->GetBufferSize(), &inputLayout);
    BREAK_ON_FAIL(result);

    Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
    result = device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &vertexShader);
    BREAK_ON_FAIL(result);

    context->VSSetShader(vertexShader.Get(), nullptr, 0);
    context->PSSetShader(pixelShader.Get(), nullptr, 0);
    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    context->IASetInputLayout(inputLayout.Get());

    constexpr FLOAT color[] = { 0.0f, 0.0f, 1.0f, 1.0f };

    assert(renderView);

    while (window.open())
    {
        window.update();
        context->ClearRenderTargetView(renderView, color);
        context->Draw(3u, 0);
        swapchain->Present(1u, 0u);
        //OutputDebugString("debug info\n");
    }

    return 0;
}

