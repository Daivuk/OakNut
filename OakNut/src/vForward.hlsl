cbuffer cbViewProjMatrix : register(b0)
{
    float4x4 viewProjMatrix;
};

cbuffer vbModelMatrix : register(b1)
{
    float4x4 modelMatrix;
};

struct sInput
{
    float3 position : V_ELEMENT0;
    float3 normal   : V_ELEMENT1;
    float2 texCoord : V_ELEMENT2;
    float3 color    : V_ELEMENT3;
};

struct sOutput
{
    float4 position : SV_POSITION;
    float3 normal   : P_ELEMENT0;
    float2 texCoord : P_ELEMENT1;
    float4 color    : P_ELEMENT2;
};

sOutput main(sInput input)
{
    sOutput output;

    output.position = mul(float4(input.position, 1), modelMatrix);
    output.position = mul(output.position, viewProjMatrix);
    output.normal = normalize(mul(float4(input.normal, 0), modelMatrix).xyz);
    output.texCoord = input.texCoord;
    output.color = float4(input.color, 1);

    return output;
}
