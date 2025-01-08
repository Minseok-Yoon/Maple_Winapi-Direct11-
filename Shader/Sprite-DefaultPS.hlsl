#include "Samplers.hlsli"
#include "Textures.hlsli"


struct VSInput
{
    float3 pos : POSITION;
    float4 color : COLOR;
    float2 uv : TEXCOORD;
};

struct VSOutput
{
    float4 pos : SV_Position;
    float4 color : COLOR;
    float2 uv : TEXCOORD;
};

float4 main(VSOutput input) : SV_Target
{
    float4 color = sprite.Sample(anisotropicSampler, input.uv);

    return color;
    //float4 texColor = sprite.Sample(anisotropicSampler, input.uv);
    //return texColor * input.color; // 텍스처 색상과 Vertex 색상을 곱함
}