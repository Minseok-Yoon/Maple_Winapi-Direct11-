#include "Samplers.hlsli"
#include "Textures.hlsli"


struct VSInput
{
    float3 pos : POSITION;
    float4 color : COLOR;
    float2 uv : TEXCOORD;
    float isLine : TEXCOORD1;
};

struct VSOutput
{
    float4 pos : SV_Position;
    float4 color : COLOR;
    float2 uv : TEXCOORD;
    float isLine : TEXCOORD1;
};

float4 main(VSOutput input) : SV_Target
{
    if (input.isLine == 1.0)
    {
        return input.color;
    }
    else
    {
        float4 color = sprite.Sample(anisotropicSampler, input.uv); // 텍스처 샘플링
        return color;
                                                                    
        // 알파(투명도)를 흑백으로 시각화: 알파==0이면 검정, 1이면 흰색
        //return float4(color.a, color.a, color.a, 1.0);
        //return color;
        //return color * 4.0; // 텍스처 색상 반환
    }
}

/*float4 color = sprite.Sample(anisotropicSampler, input.uv);

    return color;*/
    //float4 texColor = sprite.Sample(anisotropicSampler, input.uv);
    //return texColor * input.color; // 텍스처 색상과 Vertex 색상을 곱함