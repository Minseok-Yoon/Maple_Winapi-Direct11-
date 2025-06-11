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
        float4 color = sprite.Sample(anisotropicSampler, input.uv); // �ؽ�ó ���ø�
        return color; // �ؽ�ó ���� ��ȯ
    }
}

/*float4 color = sprite.Sample(anisotropicSampler, input.uv);

    return color;*/
    //float4 texColor = sprite.Sample(anisotropicSampler, input.uv);
    //return texColor * input.color; // �ؽ�ó ����� Vertex ������ ����