#include "Entity.h"
#include "EntityFactory.h"

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
