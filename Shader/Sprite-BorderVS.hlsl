#include "ConstantBuffers.hlsli"

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

VSOutput main(VSInput input)
{
    VSOutput output = (VSOutput)0.0f;

    float4 pos = mul(float4(input.pos, 1.0f), WorldMatrix); 
    float4 viewPos = mul(pos, ViewMatrix);                 
    float4 projPos = mul(viewPos, ProjectionMatrix);

    output.pos = projPos;               // 화면 좌표 변환
    output.color = input.color;         // 테두리 색상 조절 가능
    output.uv = input.uv;               // PS에서 테두리 계산에 필요

    return output;
}
