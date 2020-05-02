cbuffer PerDrawCallInfo : register(b1)
{
    row_major matrix projection;
};

cbuffer PerObjectInfo : register(b0)
{
    row_major matrix transform;
};

float4 main(float4 position : Position) : SV_Position
{
    return mul(mul(position, transform), projection);
}