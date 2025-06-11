#include "Samplers.hlsli"

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
    float2 uv = input.uv;
    float borderWidth = 0.05f;

    // 테두리 영역 체크
    if (uv.x < borderWidth || uv.x > (1.0f - borderWidth) ||
        uv.y < borderWidth || uv.y > (1.0f - borderWidth))
    {
        return float4(0.8f, 0.8f, 0.8f, 1.0f);
    }
    else
    {
        return float4(0.0f, 0.0f; 0.0f; 0.0f);
    }
}