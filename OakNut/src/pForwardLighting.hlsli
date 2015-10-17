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

float4 calculateLight(float4 diffuse, float4 lightColor, float3 lightDir, float3 normal, float4 material)
{
    // Light dot
    float lightDot = saturate(dot(normal, lightDir));
    float4 finalColor = diffuse * lightColor * lightDot * lightColor.a;

    // Specular
    float3 h = normalize(lightDir - viewDir);
    float specular = dot(normal, h);
    specular = saturate(specular);
    specular = pow(specular, material.g * 100);
    specular *= material.r;
    finalColor += lightColor * specular;

    return finalColor;
}

float4 calculateLighting(float3 worldPos, float4 diffuse, float3 normal, float4 material)
{
    float4 finalColor = float4(0, 0, 0, 1);

    // Point lights
    for (int i = 0; i < pointLightCount; ++i)
    {
        // Intensity
        float3 lightDir = pointLights[i].position - worldPos;
        float dis = dot(lightDir, lightDir);
        float disNorm = dis / (pointLights[i].radius * pointLights[i].radius);
        float attenuation = saturate(1 - disNorm);
        lightDir = normalize(lightDir);
        
        finalColor += calculateLight(diffuse, pointLights[i].color * attenuation, lightDir, normal, material);
    }

    // Directional lights
    for (i = 0; i < directionalLightCount; ++i)
    {
        finalColor += calculateLight(diffuse, directionalLights[i].color, directionalLights[i].dir, normal, material);
    }

    // Ambient
    finalColor += diffuse * ambientColor * ambientColor.a;

    // Self illumination
    finalColor += diffuse * material.b;

    return finalColor;
}
