#include <Windows.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <wrl.h>

#include <array>
#include <cassert>

#include <graphics/renderer.h>

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
    auto window = gui::Window(width, height, "draw window");
    auto hwnd = window.handle();

    auto renderer = Renderer(window);

    Microsoft::WRL::ComPtr<ID3D11Device> pDevice = renderer.device();
    Microsoft::WRL::ComPtr<IDXGISwapChain> pSwap = renderer.swapchain();
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext = renderer.context();
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTarget = renderer.target();

    /////////////////////////////////
    //create and bind vertex buffer//
    /////////////////////////////////
    const Vertex vertices[] =
    {
        DirectX::XMFLOAT4{ 0.0f, 0.5f, 1.0f, 1.0f}, DirectX::XMFLOAT4{1.0f,0.0f,0.0f, 1.0f},
        DirectX::XMFLOAT4{ 0.5f,-0.5f, 1.0f, 1.0f}, DirectX::XMFLOAT4{0.0f,1.0f,0.0f, 1.0f},
        DirectX::XMFLOAT4{-0.5f,-0.5f, 1.0f, 1.0f}, DirectX::XMFLOAT4{0.0f,0.0f,1.0f, 1.0f}
    };

    std::vector pcVertices =
    {
        PCVertex{DirectX::XMFLOAT4{ 0.0f, 0.5f, 1.0f, 1.0f}, DirectX::XMFLOAT4{1.0f,0.0f,0.0f, 1.0f}},
        PCVertex{DirectX::XMFLOAT4{ 0.5f,-0.5f, 1.0f, 1.0f}, DirectX::XMFLOAT4{0.0f,1.0f,0.0f, 1.0f}},
        PCVertex{DirectX::XMFLOAT4{-0.5f,-0.5f, 1.0f, 1.0f}, DirectX::XMFLOAT4{0.0f,0.0f,1.0f, 1.0f}}
    };

    VertexBuffer vertexBuffer(pcVertices);
    renderer.bind(vertexBuffer);

    // create vertex buffer (1 2d triangle at center of screen)
    /*Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
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
    pContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);*/

    //Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer = vertexBuffer.buffer();
    //assert(pVertexBuffer);


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

    const float red = 0.2f;
    const float green = 0.3f;
    const float blue = 0.4f;

    //MSG msg;
    while (window.open())
    {
        /*while (PeekMessage(&msg, hwnd, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }*/

        window.update();

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


