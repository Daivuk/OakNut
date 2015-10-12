#include "vForwardConstantBuffers.hlsli"

struct sInput
{
    float3 position : POSITION;
    float3 normal   : NORMAL;
    float2 texCoord : TEXCOORD;
    float3 color    : COLOR;
};

struct sOutput
{
    float4 position : SV_POSITION;
    float4 worldPos : POSITION;
    float3 normal   : NORMAL;
    float2 texCoord : TEXCOORD;
    float4 color    : COLOR;
};

sOutput main(sInput input)
{
    sOutput output;

    output.worldPos = mul(float4(input.position, 1), modelMatrix);
    output.position = mul(output.worldPos, viewProjMatrix);
    output.normal = normalize(mul(float4(input.normal, 0), modelMatrix).xyz);
    output.texCoord = input.texCoord;
    output.color = float4(input.color, 1);

    return output;
}
