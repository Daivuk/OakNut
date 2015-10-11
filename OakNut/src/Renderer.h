#pragma once
#include "Component.h"
#include "glm/glm.hpp"

namespace onut
{
    class Camera;
    class IWindow;
    class Material;
    class Mesh;

    class Renderer : public Component
    {
    public:
        Renderer();
        virtual ~Renderer();

        virtual void setCamera(Camera* pCamera) = 0;
        virtual void draw(Mesh* pMesh, Material* pMaterial, const glm::mat4& transform) = 0;
    };
}
