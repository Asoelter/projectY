#include "vertex_shader.h"

#include <d3dcompiler.h>

VertexShader::VertexShader(const std::wstring& path)
{
    Microsoft::WRL::ComPtr<ID3DBlob> errmsg;

    const auto result = D3DCompileFromFile(
        path.c_str(),
        nullptr,
        nullptr,
        "main",
        "vs_5_0",
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
        0,
        &blob_,
        &errmsg);

    if (FAILED(result))
    {
        MessageBox(NULL, "Failed to make shader", "Failed to make vertex shader", NULL);
    }
}

void VertexShader::bind(ID3D11Device* device, ID3D11DeviceContext* context)
{
    device->CreateVertexShader(blob_->GetBufferPointer(), blob_->GetBufferSize(), nullptr, &shader_);
    context->VSSetShader(shader_.Get(), nullptr, 0u);
}
