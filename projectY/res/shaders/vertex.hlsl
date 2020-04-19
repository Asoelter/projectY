struct PixelInfo
{
    float4 position : SV_Position;
    float3 color : Color;
};

PixelInfo main(float2 position : Position, float3 color : Color) 
{
    PixelInfo pi;
    pi.position = float4(position, 1.0, 1.0);
    pi.color = color;
    return pi;
}