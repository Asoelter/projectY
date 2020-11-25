struct PixelInfo
{
    float4 position : SV_Position;
    float4 color : Color;
};

cbuffer Uniforms
{
    row_major matrix transform;
};

PixelInfo main(float4 position : Position, float4 color : Color) 
{
    PixelInfo pi;
    pi.position = mul(position, transform);
    pi.color = color;
    return pi;
}