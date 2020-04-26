#include <Windows.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <wrl.h>

#include <array>
#include <cassert>

#include <graphics/renderer.h>
#include <graphics/vertex_shader.h>

#include <gui/window.h>

#include <util/uuid.h>
#include <util/error_printer.h>

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    auto window = gui::Window(800u, 600u, "draw window");

    auto renderer = Renderer(window);

    Microsoft::WRL::ComPtr<ID3D11Device> pDevice = renderer.device();
    Microsoft::WRL::ComPtr<IDXGISwapChain> pSwap = renderer.swapchain();
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext = renderer.context();
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTarget = renderer.target();

    /////////////////////////////////
    //create and bind vertex buffer//
    /////////////////////////////////
    std::vector vertices =
    {
        PCVertex{DirectX::XMFLOAT4{ 0.0f, 0.5f, 1.0f, 1.0f}, DirectX::XMFLOAT4{1.0f,0.0f,0.0f, 1.0f}},
        PCVertex{DirectX::XMFLOAT4{ 0.5f,-0.5f, 1.0f, 1.0f}, DirectX::XMFLOAT4{0.0f,1.0f,0.0f, 1.0f}},
        PCVertex{DirectX::XMFLOAT4{-0.5f,-0.5f, 1.0f, 1.0f}, DirectX::XMFLOAT4{0.0f,0.0f,1.0f, 1.0f}}
    };

    VertexBuffer vertexBuffer(vertices);


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

    pDevice->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &pPixelShader);

    // bind pixel shader
    pContext->PSSetShader(pPixelShader.Get(), nullptr, 0u);

    /////////////////////////////////
    //Create and bind Vertex Shader//
    /////////////////////////////////
    auto vshader = VertexShader(L"../../../../../projectY/res/shaders/vertex.hlsl");
    renderer.bind(vshader);
    renderer.bind(vertexBuffer);

    //////////////////////
    //configure viewport//
    //////////////////////

    const float red = 0.2f;
    const float green = 0.3f;
    const float blue = 0.4f;
    Color color(red, green, blue);

    //MSG msg;
    while (window.open())
    {
        window.update();

        renderer.clear(color);

        renderer.bind(vertexBuffer);
        renderer.draw(Topology::TriangleList);
    }
}


