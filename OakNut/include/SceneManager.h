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

        void onCreate() override;
        void onUpdate(const onut::TimeInfo& timeInfo) override;
        void onDraw() override;

        PROPERTY(std::string, StartupScene, "");

    private:
        void createEntity(Entity* pEntity);
        void updateEntity(Entity* pEntity, const onut::TimeInfo& timeInfo);
        void drawEntity(Entity* pEntity);
        void removeNonPersist(Entity* pEntity);

        Entity* m_pRootEntity = nullptr;
        bool m_bFirstSceneLoaded = false;
    };
}
