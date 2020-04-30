#include <Windows.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <wrl.h>

#include <array>
#include <cassert>
#include <cmath>

#include <graphics/constant_buffer.h>
#include <graphics/renderer.h>
#include <graphics/vertex_shader.h>

#include <gui/window.h>

#include <util/uuid.h>
#include <util/error_printer.h>

struct CBuffer
{
    DirectX::XMMATRIX transform;
};

struct ColorBuffer
{
    DirectX::XMFLOAT4 constColor;
};

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    auto window = gui::Window({ 800u, 600u }, "parent window");
    auto childWin = gui::Window({200, 150, 400, 300 }, "child window", window.handle());
    auto description = gui::Button::Descriptor(40, 20, 40, 20, "show");
    auto button = gui::Button(description);

    bool show = true;
    button.pushed.connect([&show]() {show = !show; });

    window.attach(std::move(button));

    auto renderer = Renderer(childWin);

    std::vector vertices =
    {
        PCVertex{DirectX::XMFLOAT4{ 0.0f, 0.5f, 1.0f, 1.0f}, DirectX::XMFLOAT4{1.0f,0.0f,0.0f, 1.0f}},
        PCVertex{DirectX::XMFLOAT4{ 0.5f,-0.5f, 1.0f, 1.0f}, DirectX::XMFLOAT4{0.0f,1.0f,0.0f, 1.0f}},
        PCVertex{DirectX::XMFLOAT4{-0.5f,-0.5f, 1.0f, 1.0f}, DirectX::XMFLOAT4{0.0f,0.0f,1.0f, 1.0f}}
    };

    std::vector v2 =
    {
        PCVertex{DirectX::XMFLOAT4{ 0.3f, 0.5f, 1.0f, 1.0f}, DirectX::XMFLOAT4{1.0f,0.0f,0.0f, 1.0f}},
        PCVertex{DirectX::XMFLOAT4{ 0.8f,-0.5f, 1.0f, 1.0f}, DirectX::XMFLOAT4{0.0f,1.0f,0.0f, 1.0f}},
        PCVertex{DirectX::XMFLOAT4{-0.2f,-0.5f, 1.0f, 1.0f}, DirectX::XMFLOAT4{0.0f,0.0f,1.0f, 1.0f}},
        PCVertex{DirectX::XMFLOAT4{ 0.3f, 0.5f, 1.0f, 1.0f}, DirectX::XMFLOAT4{1.0f,0.0f,0.0f, 1.0f}}
    };

    VertexBuffer vertexBuffer(vertices);
    VertexBuffer b2(v2);
    auto vshader = VertexShader(L"../../../../../projectY/res/shaders/vertex.hlsl");
    auto pshader = PixelShader(L"../../../../../projectY/res/shaders/pixel.hlsl");
    renderer.bindVertexShader(vshader);
    renderer.bindPixelShader(pshader);

    Color color(0.2f, 0.3f, 0.4f);

    float frame = 30.0f;

    CBuffer cb = { DirectX::XMMatrixRotationZ(frame) };

    ConstantBuffer<CBuffer> constBuff(cb);
    renderer.bindConstantBuffer(constBuff);
    ConstantBuffer<ColorBuffer> colorBuff({ DirectX::XMFLOAT4(0.2f, 0.3f, 0.2f, 1.0f) });
    renderer.bindConstantBuffer(colorBuff);

    while (window.open())
    {
        window.update();

        renderer.beginFrame(color);


        if (show)
        {
            renderer.bindBuffer(vertexBuffer);
            renderer.draw(Topology::TriangleList);
            renderer.bindBuffer(b2);
            renderer.draw(Topology::PolyLine);
        }

        renderer.endFrame();
        frame += 0.1f;
    }
}


