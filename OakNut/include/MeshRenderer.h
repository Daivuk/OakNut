#pragma once
#include "Component.h"
#include "Material.h"
#include "Mesh.h"

namespace onut
{
    class MeshRenderer : public Component
    {
        PROPERTY_OBJECT(onut::Mesh, Mesh, nullptr);
        PROPERTY_OBJECT(onut::Material, Material, nullptr);
    };
}
