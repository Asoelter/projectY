#ifndef PIXEL_SHADER_H
#define PIXEL_SHADER_H

#include <d3d11.h>
#include <wrl.h>

#include <string>

class PixelShader
{
public:
    PixelShader(const std::wstring& path);

    void bind(ID3D11Device* device, ID3D11DeviceContext* context);

private:
    Microsoft::WRL::ComPtr<ID3D11PixelShader> shader_;
    Microsoft::WRL::ComPtr<ID3DBlob> blob_;
};

#endif //PIXEL_SHADER_H