#include <Windows.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <wrl.h>

#include <cassert>

#include <gui/window.h>

#include <util/uuid.h>
#include <util/error_printer.h>

#include <d3d11.h>

#define GFX_THROW_INFO(func) func
#define GFX_THROW_INFO_ONLY(func) func

struct Vertex
{
    DirectX::XMFLOAT4 position;
    DirectX::XMFLOAT4 color;
};

static constexpr auto width = 800u;
static constexpr auto height = 600u;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    /*WNDCLASSEX wc = { 0 };
    wc.cbSize = sizeof(wc);
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = nullptr;
    wc.hCursor = nullptr;
    wc.hbrBackground = nullptr;
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = "wndClass";
    wc.hIconSm = nullptr;
    RegisterClassEx(&wc);

    RECT wr;
    wr.left = 100;
    wr.right = width + wr.left;
    wr.top = 100;
    wr.bottom = height + wr.top;
    if (AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE) == 0)
    {
        assert(false);
    }
    // create window & get hWnd
    HWND hwnd = CreateWindow(
        "wndClass", "Window",
        WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
        CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
        nullptr, nullptr, hInstance, nullptr
    );
    // check for error
    if (!hwnd)
    {
        assert(false);
    }
    // newly created windows start off as hidden
    ShowWindow(hwnd, SW_SHOWDEFAULT);*/
    // create graphics object

    auto window = gui::Window(width, height, "draw window");
    auto hwnd = window.handle();

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
    sd.OutputWindow = hwnd;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    sd.Flags = 0;

    UINT swapCreateFlags = 0u;
#ifdef DEBUG
    swapCreateFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
    Microsoft::WRL::ComPtr<IDXGISwapChain> pSwap;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTarget;

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
        &pSwap,
        &pDevice,
        nullptr,
        &pContext
    );
    // gain access to texture subresource in swap chain (back buffer)
    Microsoft::WRL::ComPtr<ID3D11Resource> pBackBuffer;
    pSwap->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer);
    pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &pTarget);

    const float red = 0.2f;
    const float green = 0.3f;
    const float blue = 0.4f;

    /////////////////////////////////
    //create and bind vertex buffer//
    /////////////////////////////////
    const Vertex vertices[] =
    {
        DirectX::XMFLOAT4{ 0.0f, 0.5f, 1.0f, 1.0f}, DirectX::XMFLOAT4{1.0f,0.0f,0.0f, 1.0f},
        DirectX::XMFLOAT4{ 0.5f,-0.5f, 1.0f, 1.0f}, DirectX::XMFLOAT4{0.0f,1.0f,0.0f, 1.0f},
        DirectX::XMFLOAT4{-0.5f,-0.5f, 1.0f, 1.0f}, DirectX::XMFLOAT4{0.0f,0.0f,1.0f, 1.0f}
    };

    // create vertex buffer (1 2d triangle at center of screen)
    Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
    D3D11_BUFFER_DESC bd = {};
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.CPUAccessFlags = 0u;
    bd.MiscFlags = 0u;
    bd.ByteWidth = sizeof(vertices);
    bd.StructureByteStride = sizeof(Vertex);
    D3D11_SUBRESOURCE_DATA srd = {};
    srd.pSysMem = vertices;
    GFX_THROW_INFO(pDevice->CreateBuffer(&bd, &srd, &pVertexBuffer));

    const UINT stride = sizeof(Vertex);
    const UINT offset = 0u;
    pContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);

    ////////////////////////////////
    //Create and bind pixel shader//
    ////////////////////////////////
    Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;

    Microsoft::WRL::ComPtr<ID3DBlob> blob;
    Microsoft::WRL::ComPtr<ID3DBlob> errmsg;
    auto result = D3DCompileFromFile(
        L"../../../../../projectY/res/shaders/pixel.hlsl",
        nullptr,
        nullptr,
        "main",
        "ps_5_0",
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
        0,
        &blob,
        &errmsg
    );

    GFX_THROW_INFO(pDevice->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &pPixelShader));

    // bind pixel shader
    pContext->PSSetShader(pPixelShader.Get(), nullptr, 0u);

    /////////////////////////////////
    //Create and bind Vertex Shader//
    /////////////////////////////////
    Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;

    result = D3DCompileFromFile(
        L"../../../../../projectY/res/shaders/vertex.hlsl",
        nullptr,
        nullptr,
        "main",
        "vs_5_0",
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
        0,
        &blob,
        &errmsg);

    GFX_THROW_INFO(pDevice->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &pVertexShader));

    // bind vertex shader
    pContext->VSSetShader(pVertexShader.Get(), nullptr, 0u);

    ////////////////////////////////////////////
    //Describe the layout of the vertex buffer//
    ////////////////////////////////////////////
    Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;
    const D3D11_INPUT_ELEMENT_DESC ied[] =
    {
        { "Position",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
        { "Color",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,16u,D3D11_INPUT_PER_VERTEX_DATA,0 },
    };
    GFX_THROW_INFO(pDevice->CreateInputLayout(
        ied, (UINT)std::size(ied),
        blob->GetBufferPointer(),
        blob->GetBufferSize(),
        &pInputLayout
    ));

    // bind vertex layout
    pContext->IASetInputLayout(pInputLayout.Get());

    // bind render target
    pContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), nullptr);

    // Set primitive topology to triangle list (groups of 3 vertices)
    pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    //////////////////////
    //configure viewport//
    //////////////////////
    D3D11_VIEWPORT vp;
    vp.Width = 800;
    vp.Height = 600;
    vp.MinDepth = 0;
    vp.MaxDepth = 1;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    pContext->RSSetViewports(1u, &vp);

    MSG msg;
    while (true)
    {
        while (PeekMessage(&msg, hwnd, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        //clearBuffer
        const float color[] = { red,green,blue,1.0f };
        pContext->ClearRenderTargetView(pTarget.Get(), color);

        pContext->Draw((UINT)std::size(vertices), 0u);

        //endFrame
        pSwap->Present(1u, 0u);
    }
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CLOSE:
    {
        PostQuitMessage(0);
        return 0;
    }break;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}


