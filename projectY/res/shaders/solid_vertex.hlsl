cbuffer PerDrawCallInfo : register(b0)
{
    row_major matrix projection;
    row_major matrix view;
};

cbuffer PerObjectInfo : register(b1)
{
    row_major matrix transform;
};

float4 main(float4 position : Position) : SV_Position
{
    return mul(mul(position, projection), transform);
}
