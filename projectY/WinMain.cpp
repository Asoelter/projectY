#include <Windows.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <wrl.h>

#include <array>
#include <cassert>
#include <cmath>

#include <graphics/constant_buffer.h>
#include <graphics/mesh.h>
#include <graphics/pixel_shader.h>
#include <graphics/renderer.h>
#include <graphics/vertex_shader.h>

#include <gui/window.h>

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    auto window = gui::Window({ 800u, 600u }, "parent window");
    auto childWin = gui::Window({200, 150, 400, 300 }, "child window", window.handle());
    auto description = gui::Button::Descriptor(40, 20, 40, 20, "show");
    auto button = gui::Button(description);

    bool show = true;
    button.pushed.connect([&show]() {show = !show; });

    window.attach(std::move(button));

    auto projection = DirectX::XMMatrixTranslation(-4.0f, -4.0f, 0.0f) * DirectX::XMMatrixOrthographicLH(8.0f, 8.0f, 0.0f, 2.0f) ;
    auto renderer = Renderer(childWin, projection);

    std::vector vertices =
    {
        PVertex{DirectX::XMFLOAT4{ 2.0f, 4.0f, 1.0f, 1.0f}},
        PVertex{DirectX::XMFLOAT4{ 4.0f, 0.0f, 1.0f, 1.0f}},
        PVertex{DirectX::XMFLOAT4{ 0.0f, 0.0f, 1.0f, 1.0f}}
    };
    auto vshader = VertexShader(shaderSource(L"solid_vertex.hlsl"));
    auto pshader = PixelShader(shaderSource(L"solid_pixel.hlsl"));
    renderer.bindVertexShader(vshader);
    renderer.bindPixelShader(pshader);

    Color color = Color::LightGrey();

    //MatrixBuffer cb = { DirectX::XMMatrixRotationZ(30.0f) };
    MatrixBuffer cb = { DirectX::XMMatrixIdentity() };
    ConstantBuffer<MatrixBuffer> mb(cb, BufferType::Vertex);

    Mesh mesh(vertices);

    while (window.open())
    {
        window.update();

        renderer.beginFrame(color);

        if (show)
        {
            renderer.bindConstantBuffer(mb);
            renderer.draw(mesh);
        }

        renderer.endFrame();
    }
}


