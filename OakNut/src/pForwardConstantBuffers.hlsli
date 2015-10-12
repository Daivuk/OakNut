cbuffer cbView : register(b0)
{
    float3 viewDir;
    float cbView_padding;
};

cbuffer cbMaterial : register(b1)
{
    float4 tint;
    float reflectivity;
    float metallic;
    float roughness;
    float cbMaterial_padding;
};
