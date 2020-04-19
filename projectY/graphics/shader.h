#ifndef SHADER_H
#define SHADER_H

#include <d3d11.h>
#include <wrl.h>

#include <string>

class Shader
{
public:
    Shader(const std::string& vertexPath, const std::string& pixelPath);

    void bind(ID3D11Device* device, ID3D11DeviceContext* context);
private:
    Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader_;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader_;
};

#endif //SHADER_H