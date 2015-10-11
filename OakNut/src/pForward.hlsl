SamplerState samplerState : register(s0);
Texture2D Diffuse : register(t0);
Texture2D NormalMap : register(t1);
Texture2D MaterialMap : register(t2);

cbuffer cbMaterial : register(b0)
{
    float4 tint;
    float reflectivity;
    float metallic;
    float roughness;
    float padding;
};

struct sInput
{
    float4 position : SV_POSITION;
    float3 normal   : P_ELEMENT0;
    float2 texCoord : P_ELEMENT1;
    float4 color    : P_ELEMENT2;
};

float4 main(sInput input) : SV_TARGET
{
    const float3 lightDir = -normalize(float3(-1, 1, -1));

    float lightIntensity = saturate(dot(lightDir, input.normal));

    return tint * lightIntensity;
}
