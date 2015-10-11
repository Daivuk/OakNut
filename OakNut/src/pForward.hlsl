SamplerState samplerState : register(s0);

struct sInput
{
    float4 position : SV_POSITION;
    float3 normal   : P_ELEMENT0;
    float2 texCoord : P_ELEMENT1;
    float4 color    : P_ELEMENT2;
};

float4 main(sInput input) : SV_TARGET
{
	return float4(1.0f, 0.0f, 0.0f, 1.0f);
}
