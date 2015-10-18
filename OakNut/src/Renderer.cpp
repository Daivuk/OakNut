#include "AmbientLight.h"
#include "Camera.h"
#include "ComponentManager.h"
#include "DirectionalLight.h"
#include "Entity.h"
#include "MeshRenderer.h"
#include "PointLight.h"
#include "Renderer.h"
#include "SceneManager.h"

#include <algorithm>
#include <glm/gtx/norm.hpp>

onut::Renderer::Renderer()
{
    m_solids.reserve(4096); // This will grow automatically anyway
    m_pointLights.reserve(1024);
    m_pointLightsPassThrough.reserve(MAX_POINT_LIGHTS);
}

onut::Renderer::~Renderer()
{
}

void onut::Renderer::onDraw()
{
    auto pComponentManager = getComponentManager();
    if (!pComponentManager) return;
    auto pSceneManager = pComponentManager->getComponent<SceneManager>();
    if (!pSceneManager) return;
    auto pRootNode = pSceneManager->getRootEntity();
    if (!pRootNode) return;

    // This will clear buffers and initialize the frame
    begin();

    // Find all renderables
    m_solids.clear();
    m_pointLights.clear();
    m_directionalLights.clear();
    m_ambientColor = glm::vec4(0, 0, 0, 0);
    m_pCamera = nullptr;
    pRootNode->visit([this](Entity* pEntity)
    {
        if (pEntity->getVisible())
        {
            collectRenderable(pEntity);
            return true;
        }
        return false;
    });
    collectRenderable(pRootNode);

    // Set camera
    setCamera(m_pCamera);

    // Draw solids
    for (auto& renderAction : m_solids)
    {
        if (!renderAction.pMesh || !renderAction.pMaterial || !renderAction.pTransform) continue;

        // Sort point lights by distance
        const auto& position = (*renderAction.pTransform)[3];

        // Setup lights
        m_pointLightsPassThrough.clear();
        for (auto pPointLight : m_pointLights)
        {
            const auto& posA = pPointLight->getEntity()->getWorldMatrix()[3];
            auto disA = glm::distance2(posA, position);
            if (disA < pPointLight->getRadius() * pPointLight->getRadius())
            {
                m_pointLightsPassThrough.push_back(pPointLight);
            }
        }
        m_directionalLightsPassThrough.clear();
        for (auto pDirectionalLight : m_directionalLights)
        {
            m_directionalLightsPassThrough.push_back(pDirectionalLight);
        }

        // Draw
        draw(renderAction.pMesh, renderAction.pMaterial, *renderAction.pTransform, m_ambientColor, m_pointLightsPassThrough, m_directionalLightsPassThrough);
    }

    // Draw transparents

    // Swap the buffers
    end();
}

void onut::Renderer::collectRenderable(Entity* pEntity)
{
    // Add mesh
    auto pMeshRenderer = pEntity->getComponent<MeshRenderer>();
    if (pMeshRenderer)
    {
        if (!pMeshRenderer->getMesh() || !pMeshRenderer->getMaterial()) return;
        m_solids.push_back({pMeshRenderer->getMesh(), pMeshRenderer->getMaterial(), &pEntity->getWorldMatrix()});
    }

    // Select active camera
    auto pCamera = pEntity->getComponent<Camera>();
    if (pCamera)
    {
        if (pCamera->getActive())
        {
            m_pCamera = pCamera;
        }
    }

    // Add light
    auto pPointLight = pEntity->getComponent<PointLight>();
    if (pPointLight)
    {
        m_pointLights.push_back(pPointLight);
    }
    auto pDirectionalLight = pEntity->getComponent<DirectionalLight>();
    if (pDirectionalLight)
    {
        m_directionalLights.push_back(pDirectionalLight);
    }
    auto pAmbientLight = pEntity->getComponent<AmbientLight>();
    if (pAmbientLight)
    {
        m_ambientColor += pAmbientLight->getColor();
    }
}
