cbuffer cbViewProjMatrix : register(b0)
{
    float4x4 viewProjMatrix;
};

cbuffer vbModelMatrix : register(b1)
{
    float4x4 modelMatrix;
};
