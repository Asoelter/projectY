#include "camera.h"

Camera::Camera(const WorldDescriptor& world, const ViewDescriptor& view)
    : projection_()
    , view_()
{
    const auto xmEye = DirectX::XMVectorSet(view.eye.x, view.eye.y, view.eye.z, 1.0f);
    const auto xmCenter = DirectX::XMVectorSet(view.center.x, view.center.y, view.center.z, 1.0f);
    const auto xmUp = DirectX::XMVectorSet(view.up.x, view.up.y, view.up.z, 1.0f);

    view_ = DirectX::XMMatrixLookAtLH(xmEye, xmCenter, xmUp);
}

void Camera::pan(const math::vec3<float>& direction)
{
}

void Camera::zoom(float amount)
{
}
