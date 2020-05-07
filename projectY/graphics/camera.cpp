#include "camera.h"

namespace
{

DirectX::XMMATRIX makeProjection(const WorldDescriptor& world)
{
    const auto width  = world.xmax - world.xmin;
    const auto height = world.ymax - world.ymin;
    const auto deltax = (world.xmax + world.xmin) / 2.0f;
    const auto deltay = (world.ymax + world.ymin) / 2.0f;
    const auto deltaz = (world.zmax + world.zmin) / 2.0f;

    const auto rawProjection = DirectX::XMMatrixOrthographicLH(width, height, world.zmin, world.zmax);
    const auto projTrans = DirectX::XMMatrixTranslation(-deltax, -deltay, -deltaz);

    return projTrans * rawProjection;
}

DirectX::XMMATRIX makeView(const ViewDescriptor& view)
{

    const auto xmEye = view.eye.toXMVector();
    const auto xmCenter = view.center.toXMVector();
    const auto xmUp = view.up.toXMVector();

    return DirectX::XMMatrixLookAtLH(xmEye, xmCenter, xmUp);
}
}

Camera::Camera(const WorldDescriptor& world, const ViewDescriptor& view)
    : eye_(view.eye)
    , center_(view.center)
    , up_(view.up)
    , projection_(makeProjection(world))
    , view_(makeView(view))
    , constBuffer_({projection_, view_}, BufferType::Vertex)
{
}

void Camera::bind(ID3D11Device* device, ID3D11DeviceContext* context)
{
    constBuffer_.bind(device, context);
}

void Camera::pan(const math::vec3<float>& direction)
{
    eye_ += direction;
    center_ += direction;
    ViewDescriptor view(eye_, center_, up_);
    view_ = makeView(view);
    constBuffer_ = ConstantBuffer<CameraDataBuffer>({ projection_, view_ }, BufferType::Vertex);
}

void Camera::zoom(float amount)
{
}

//Produces a camera who's view ranges from 0 <= x <= width
//and 0 <= y <= height
Camera firstQuadOrthoCamera(float width, float height, float depth)
{
    WorldDescriptor world;
    world.xmin = 0.0f;
    world.xmax = width;
    world.ymin = 0; 
    world.ymax = height;
    world.zmin = 0;
    world.zmax = depth;

    const auto eye    = math::vec3(0.0f, 0.0f, -1.0f);
    const auto center = math::vec3(0.0f, 0.0f, 0.0f);
    const auto up     = math::vec3(0.0f, 1.0f, 0.0f);

    ViewDescriptor view(eye, center, up);

    return Camera(world, view);
}

//Produces a camera who's view ranges from -width <= x <= width
//and -height <= y <= height
Camera fourQuadOrthoCamera(float width, float height)
{
    WorldDescriptor world;
    world.xmin = -1.0f * width;
    world.xmax = width;
    world.ymin = -1.0f * height; 
    world.ymax = height;
    world.zmin = -1;
    world.zmax = 0;

    const auto eye    = math::vec3(0.0f, 0.0f, -1.0f);
    const auto center = math::vec3(0.0f, 0.0f, 0.0f);
    const auto up     = math::vec3(0.0f, 1.0f, 0.0f);

    ViewDescriptor view(eye, center, up);

    return Camera(world, view);
}
