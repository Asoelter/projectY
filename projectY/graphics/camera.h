#ifndef CAMERA_H
#define CAMERA_H

#include <DirectXMath.h>

class Camera
{
public:

    void pan(/*???*/);
    void zoom(float amount);

private:
    DirectX::XMMATRIX projection_;
    DirectX::XMMATRIX view_;
};

#endif //CAMERA_H