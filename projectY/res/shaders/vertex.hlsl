struct PixelInfo
{
    float4 position : SV_Position;
    float4 color : Color;
};

PixelInfo main(float4 position : Position, float4 color : Color) 
{
    PixelInfo pi;
    pi.position = position;
    pi.color = color;
    return pi;
}