#ifndef CAMERA_H
#define CAMERA_H

#include <d3d11.h>
#include <DirectXMath.h>

#include <math/vec3.h>

#include "constant_buffer.h"

struct WorldDescriptor
{
    float xmin;
    float xmax;
    float ymin;
    float ymax;
    float zmin;
    float zmax;
};

struct ViewDescriptor
{
    ViewDescriptor(const math::vec3<float>& e, const math::vec3<float>& c, const math::vec3<float>& u)
        : eye(e)
        , center(c)
        , up(u)
    {

    }
    math::vec3<float> eye;
    math::vec3<float> center;
    math::vec3<float> up;
};

class Camera
{
public:
    Camera(const WorldDescriptor& world, const ViewDescriptor& view);

    void bind(ID3D11Device* device, ID3D11DeviceContext* context);

    void move(const math::vec3<float>& direction);
    void pan(const math::vec3<float>& direction);
    void zoom(float amount);

private:
    struct CameraDataBuffer
    {
        DirectX::XMMATRIX projection;
        DirectX::XMMATRIX view;
    };

    math::vec3<float>                eye_;
    math::vec3<float>                center_;
    math::vec3<float>                up_;
    DirectX::XMMATRIX                projection_;
    DirectX::XMMATRIX                view_;
    ConstantBuffer<CameraDataBuffer> constBuffer_;
};

Camera firstQuadOrthoCamera(float width, float height, float depth);
Camera fourQuadOrthoCamera(float width, float height);

#endif //CAMERA_H