#include "ContentManager.h"
#include "Entity.h"
#include "EntityFactory.h"
#include "Game.h"
#include "Material.h"
#include "Mesh.h"
#include "MeshRenderer.h"
#include "PointLight.h"

std::unordered_map<std::string, onut::EntityFactory::EntityPool> onut::EntityFactory::s_pools;

void onut::EntityFactory::clear()
{
    s_pools.clear();
}

onut::Entity* onut::EntityFactory::createFromPrefab(const std::string& prefabName)
{
    auto it = s_pools.find(prefabName);
    if (it == s_pools.end()) return nullptr;
    EntityPool& pool = it->second;
    int end = pool.nextIndex;
    do
    {
        EntityPrefab& prefab = pool.prefabs[pool.nextIndex];
        pool.nextIndex = (pool.nextIndex + 1) % pool.count;
        if (!prefab.bUsed)
        {
            prefab.bUsed = true;
            Entity* pEntity = new Entity(*prefab.pPrefab);
            pEntity->retain();
            return pEntity;
        }
    } while (pool.nextIndex != end);
    return nullptr;
}

bool onut::EntityFactory::createPrefab(Entity* pPrefab, const std::string& prefabName, int poolCount)
{
    auto it = s_pools.find(prefabName);
    if (it != s_pools.end()) return false;
    if (poolCount <= 1) return false;
    s_pools[prefabName] = EntityPool(pPrefab, poolCount);
    return true;
}

std::vector<std::string> onut::EntityFactory::getPrefabNames()
{
    std::vector<std::string> names;
    for (auto& kv : s_pools)
    {
        names.push_back(kv.first);
    }
    return std::move(names);
}

bool onut::EntityFactory::deletePrefab(const std::string& prefabName)
{
    auto it = s_pools.find(prefabName);
    if (it == s_pools.end()) return false;
    s_pools.erase(it);
    return true;
}

onut::EntityFactory::EntityPrefab::~EntityPrefab()
{
    pPrefab->release();
}

onut::EntityFactory::EntityPool::EntityPool(Entity* pPrefab, int poolCount)
    : count(poolCount)
{
    prefabs = new EntityPrefab[count];
    for (decltype(count) i = 0; i < count; ++i)
    {
        EntityPrefab& prefab = prefabs[i];
        prefab.pPrefab = new Entity(*pPrefab);
        prefab.pPrefab->retain();
    }
}

onut::EntityFactory::EntityPool::~EntityPool()
{
    if (prefabs) delete[] prefabs;
}

// Helpers
onut::Entity* onut::EntityFactory::createPointLight(const glm::vec3& position, float radius, const glm::vec4& color)
{
    auto pEntity = new onut::Entity();
    pEntity->setPosition(position);
    auto pPointLight = pEntity->addComponent<onut::PointLight>();
    pPointLight->setRadius(radius);
    pPointLight->setColor(color);

    return pEntity;
}

onut::Entity* onut::EntityFactory::createMesh(const glm::vec3& position, const std::string& meshFilename, const std::string& materialFilename)
{
    auto pContentManager = onut::Game::getGame()->getComponent<onut::ContentManager>();

    auto pMesh = pContentManager->getResource<onut::Mesh>(meshFilename);
    if (!pMesh)
    {
        pMesh = onut::Mesh::create();
        auto bRet = pMesh->load(meshFilename);
        assert(bRet);
        pContentManager->addResource(meshFilename, pMesh);
    }

    auto pMaterial = pContentManager->getResource<onut::Material>(materialFilename);
    if (!pMaterial)
    {
        pMaterial = new onut::Material();
        auto bRet = pMaterial->loadPropertiesFromFile(materialFilename);
        assert(bRet);
        pContentManager->addResource(materialFilename, pMaterial);
    }

    auto pEntity = new onut::Entity();
    pEntity->setPosition(position);

    auto pMeshRenderer = pEntity->addComponent<onut::MeshRenderer>();
    pMeshRenderer->setMesh(pMesh);
    pMeshRenderer->setMaterial(pMaterial);

    return pEntity;
}
