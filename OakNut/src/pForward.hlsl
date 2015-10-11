SamplerState samplerState : register(s0);
Texture2D diffuseTexture : register(t0);
Texture2D mormalMapTexture : register(t1);
Texture2D materialMapTexture : register(t2);

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

struct sInput
{
    float4 position : SV_POSITION;
    float3 normal   : P_ELEMENT0;
    float2 texCoord : P_ELEMENT1;
    float4 color    : P_ELEMENT2;
};

// Diffuse
//Charcoal       0.04
//Fresh asphalt  0.04
//Worn asphalt   0.12
//Bare soil      0.17
//Green grass    0.25
//Desert sand    0.40
//New concrete   0.55
//Ocean Ice      0.5–0.7
//Fresh snow     0.80–0.90

// Specular
//Quartz    0.045593921
//ice       0.017908907
//Water     0.020373188
//Alcohol   0.01995505
//Glass     0.04
//Milk      0.022181983
//Ruby      0.077271957
//Crystal   0.111111111
//Diamond   0.171968833
//Skin      0.028

//              R            G            B
//Silver      0.971519    0.959915    0.915324
//Aluminium   0.913183    0.921494    0.924524
//Gold        1           0.765557    0.336057
//Copper      0.955008    0.637427    0.538163
//Chromium    0.549585    0.556114    0.554256
//Nickel      0.659777    0.608679    0.525649
//Titanium    0.541931    0.496791    0.449419
//Cobalt      0.662124    0.654864    0.633732
//Platinum    0.672411    0.637331    0.585456

static const float3 lightDir = -normalize(float3(-1, 1, -2));
static const float4 lightColor = float4(1, 1, 1, 1);

float4 calculateLight(float4 diffuse, float3 normal)
{
    float intensity = dot(normal, lightDir);
    intensity = saturate(intensity);

    return diffuse * lightColor * intensity;
}

float4 calculateSpecular(float4 material, float3 normal)
{
    float3 h = normalize(lightDir - viewDir);
    float intensity = dot(normal, h);
    intensity = saturate(intensity);
    intensity = pow(intensity, material.g * 100);
    intensity *= material.r;

    return lightColor * intensity;
}

float4 calculateFresnel(float4 material, float3 normal)
{
    float intensity = dot(normal, -viewDir);
    intensity = 1 - saturate(intensity);
    intensity = pow(intensity, material.g * 20);
    intensity *= material.r;

    return lightColor * intensity;
}

float4 main(sInput input) : SV_TARGET
{
    float4 diffuse = diffuseTexture.Sample(samplerState, input.texCoord);
    float4 normalMap = mormalMapTexture.Sample(samplerState, input.texCoord);
    float4 materialMap = materialMapTexture.Sample(samplerState, input.texCoord);
    diffuse *= tint;

    float4 finalColor = calculateLight(diffuse, input.normal);
    finalColor += calculateSpecular(materialMap, input.normal);
    //finalColor += calculateFresnel(materialMap, input.normal);

    return finalColor;
}
