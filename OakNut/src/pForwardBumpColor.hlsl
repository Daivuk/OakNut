#include "pForwardConstantBuffers.hlsli"
#include "pForwardLighting.hlsli"

SamplerState samplerState : register(s0);
Texture2D diffuseTexture : register(t0);
Texture2D mormalMapTexture : register(t1);
Texture2D materialMapTexture : register(t2);

struct sInput
{
    float4 position : SV_POSITION;
    float3 normal   : NORMAL;
    float3 tangent  : TANGENT;
    float3 binormal : BINORMAL;
    float2 texCoord : TEXCOORD;
    float4 color    : COLOR;
};

float4 main(sInput input) : SV_TARGET
{
    float4 diffuse = diffuseTexture.Sample(samplerState, input.texCoord);
    float4 normalMap = mormalMapTexture.Sample(samplerState, input.texCoord);
    float4 materialMap = materialMapTexture.Sample(samplerState, input.texCoord);
    diffuse *= tint * input.color;

    float3 normal = normalize(normalMap.x * input.tangent + normalMap.y * input.binormal + normalMap.z * input.normal);

        return calculateLighting(diffuse, normal, materialMap);
}
