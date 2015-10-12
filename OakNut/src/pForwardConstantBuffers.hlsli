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

struct sPointLight
{
    float3 position;
    float radius;
    float4 color;
};

struct sDirectionalLight
{
    float3 dir;
    float sDirectionalLight_padding;
    float4 color;
};

static const int MAX_POINT_LIGHTS = 4;
static const int MAX_DIRECTIONAL_LIGHTS = 2;

cbuffer cbLights : register(b2)
{
    int pointLightCount;
    int directionalLightCount;
    float2 cbLights_padding1;
    sPointLight pointLights[MAX_POINT_LIGHTS];
    sDirectionalLight directionalLights[MAX_DIRECTIONAL_LIGHTS];
};
