cbuffer Uniforms
{
    float4 constColor;
};

float4 main(float4 position : SV_Position, float4 color : Color) : SV_Target
{
    color.r = constColor.r;
    return color;
}