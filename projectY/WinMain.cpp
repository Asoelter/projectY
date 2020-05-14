#include <Windows.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <wrl.h>

#include <array>
#include <cassert>
#include <cmath>

#include <graphics/camera.h>
#include <graphics/constant_buffer.h>
#include <graphics/mesh.h>
#include <graphics/pixel_shader.h>
#include <graphics/renderer.h>
#include <graphics/vertex_shader.h>

#include <gui/button.h>
#include <gui/mouse.h>
#include <gui/keyboard.h>
#include <gui/window.h>

#include <math/vec3.h>

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    auto window = gui::Window({ 800u, 600u }, "parent window");
    auto childWin = gui::Window({200, 150, 400, 300 }, "child window", window.handle());
    auto description = gui::Button::Descriptor(40, 20, 40, 20, "show");
    auto button = gui::Button(description);

    bool show = true;
    connect(button.pushed, Slot<>([&show]() {show = !show; }));

    window.attach(std::move(button));

    auto renderer = Renderer(childWin);
    auto camera = firstQuadOrthoCamera(8.0f, 8.0f, 2.0f);

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

    Mesh mesh(vertices);


    connect(gui::Keyboard::upArrowKeyPressed,    Slot<>([&mesh]() {mesh.translate(0.0f, 0.01f); }));
    connect(gui::Keyboard::rightArrowKeyPressed, Slot<>([&mesh]() {mesh.translate(0.01f, 0.0f); }));
    connect(gui::Keyboard::downArrowKeyPressed,  Slot<>([&mesh]() {mesh.translate(0.0f, -0.01f); }));
    connect(gui::Keyboard::leftArrowKeyPressed,  Slot<>([&mesh]() {mesh.translate(-0.01f, 0.0f); }));
    //connect(gui::Mouse::leftButtonDoubleClicked, gui::Mouse::SlotType([](int, int) {MessageBox(NULL, "dblclick", "dblclick", NULL); }));

    while (window.open())
    {
        window.update();

        renderer.beginFrame(color);
        renderer.bindCamera(camera);

        if (show)
        {
            renderer.draw(mesh);
        }

        renderer.endFrame();
    }
}


