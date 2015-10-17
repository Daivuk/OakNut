#pragma once
#include "Component.h"

namespace onut
{
    class Entity;

    class SceneManager : public Component
    {
    public:
        SceneManager();
        virtual ~SceneManager();

        void loadScene(const std::string& filename);
        void loadSceneAsync(const std::string& filename);

        Entity* getRootEntity() const;

        void onCreate() override;
        void onUpdate(const onut::TimeInfo& timeInfo) override;

        PROPERTY(std::string, StartupScene, "");

    private:
        void updateEntity(Entity* pEntity, const onut::TimeInfo& timeInfo);
        void removeNonPersist(Entity* pEntity);
        void removeWeakReferences(Entity* pToDereference);

        Entity* m_pRootEntity = nullptr;
        bool m_bFirstSceneLoaded = false;
    };
}
