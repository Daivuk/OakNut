#pragma once
#include <string>
#include <unordered_map>
#include <vector>

namespace onut
{
    class Entity;

    class EntityFactory
    {
    public:
        static void clear();
        static Entity* createFromPrefab(const std::string& prefabName);
        static bool createPrefab(Entity* pPrefab, const std::string& prefabName, int poolCount = 1);
        static std::vector<std::string> getPrefabNames();
        static bool deletePrefab(const std::string& prefabName);

        // Common entity helpers
        static Entity* createPointLight(const glm::vec3& position, float radius, const glm::vec4& color);
        static Entity* createMesh(const glm::vec3& position, const std::string& meshFilename, const std::string& materialFilename);

    private:
        struct EntityPrefab
        {
            ~EntityPrefab();
            bool bUsed = false;
            Entity* pPrefab = nullptr;
        };
        struct EntityPool
        {
            EntityPool() {}
            EntityPool(Entity* pPrefab, int poolCount);
            ~EntityPool();
            int count = 0;
            int nextIndex = 0;
            EntityPrefab* prefabs = nullptr;
        };

        EntityFactory() {}

        static std::unordered_map<std::string, EntityPool> s_pools;
    };
}
