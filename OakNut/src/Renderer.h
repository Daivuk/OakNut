#pragma once
#include "Component.h"
#include "glm/glm.hpp"

namespace onut
{
    class Camera;
    class DirectionalLight;
    class IWindow;
    class Material;
    class Mesh;
    class PointLight;

    class Renderer : public Component
    {
    public:
        static const int32_t MAX_POINT_LIGHTS = 4;
        static const int32_t MAX_DIRECTIONAL_LIGHTS = 2;

        Renderer();
        virtual ~Renderer();

        virtual void onDraw() override;

        virtual void setCamera(Camera* pCamera) = 0;
        virtual void draw(Mesh* pMesh, 
                          Material* pMaterial, 
                          const glm::mat4& transform, 
                          const std::vector<onut::PointLight*>& pointLights,
                          const std::vector<onut::DirectionalLight*>& directionalLights) = 0;

    protected:
        struct sMeshes
        {
            onut::Mesh* pMesh;
            onut::Material* pMaterial;
            const glm::mat4* pTransform;
        };

        void collectRenderable(Entity* pEntity);

        virtual void begin() = 0;
        virtual void end() = 0;

        std::vector<sMeshes> m_solids;
        std::vector<onut::PointLight*> m_pointLights;
        std::vector<onut::PointLight*> m_pointLightsPassThrough;
        std::vector<onut::DirectionalLight*> m_directionalLights;
        std::vector<onut::DirectionalLight*> m_directionalLightsPassThrough;
        Camera* m_pCamera = nullptr;
    };
}
