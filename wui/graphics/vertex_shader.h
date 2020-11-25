#ifndef SHADER_H
#define SHADER_H

#include <wrl.h>

#include <string>

#include "vertex_buffer.h"

inline std::wstring shaderSource(const std::wstring fileName)
{
    return L"../../../../../projectY/res/shaders/" + fileName;
}

class VertexShader
{
public:
    VertexShader(const std::wstring& path);

    void bind(ID3D11Device* device, ID3D11DeviceContext* context);

    //debug functions for roughing in 
    Microsoft::WRL::ComPtr<ID3D11VertexShader> rawShader() { return shader_; }
    Microsoft::WRL::ComPtr<ID3DBlob> blob() { return blob_; }

private:
    Microsoft::WRL::ComPtr<ID3D11VertexShader> shader_;
    Microsoft::WRL::ComPtr<ID3DBlob> blob_;
};

#endif //SHADER_H