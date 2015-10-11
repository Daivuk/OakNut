#pragma once
#include "PropertyManager.h"
#include "Resource.h"
#include "Texture.h"

namespace onut
{
    class Material 
        : public Resource
    {
    public:
    private:
        PROPERTY(glm::vec4, Tint, glm::vec4(1.f, 1.f, 1.f, 1.f));
        PROPERTY(float, Reflectivity, 0.f);
        PROPERTY(float, Metallic, 0.f);
        PROPERTY(float, Roughness, 0.f);
        PROPERTY_OBJECT(onut::Texture, Diffuse, nullptr);
        PROPERTY_OBJECT(onut::Texture, NormalMap, nullptr);
        PROPERTY_OBJECT(onut::Texture, MaterialMap, nullptr);
    };
}
