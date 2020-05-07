#ifndef CAMERA_H
#define CAMERA_H

#include <DirectXMath.h>

#include <math/vec3.h>

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
    math::vec3<float> eye;
    math::vec3<float> center;
    math::vec3<float> up;
};

class Camera
{
public:
    Camera(const WorldDescriptor& world, const ViewDescriptor& view);

    void pan(const math::vec3<float>& direction);
    void zoom(float amount);

private:
    DirectX::XMMATRIX projection_;
    DirectX::XMMATRIX view_;
};

#endif //CAMERA_H