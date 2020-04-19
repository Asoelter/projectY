float4 main(float4 position : SV_Position, float3 color : Color) : SV_Target
{
    return float4(color, 1.0f);
}