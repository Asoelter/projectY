cbuffer buff
{
    row_major matrix transform;
};

float4 main(float4 position : Position) : SV_Position
{
    return mul(position, transform);
}
