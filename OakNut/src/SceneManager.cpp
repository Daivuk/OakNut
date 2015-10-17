#include "Dispatcher.h"
#include "Entity.h"
#include "Game.h"
#include "SceneManager.h"

#include <future>

onut::SceneManager::SceneManager()
{
    m_pRootEntity = new onut::Entity();
    m_pRootEntity->retain();
}

onut::SceneManager::~SceneManager()
{
    m_pRootEntity->release();
}

void onut::SceneManager::onCreate()
{
}

void onut::SceneManager::onUpdate(const onut::TimeInfo& timeInfo)
{
    if (!m_bFirstSceneLoaded)
    {
        m_bFirstSceneLoaded = true;
        // Load the default scene
        if (!getStartupScene().empty())
        {
            loadScene(getStartupScene());
        }
    }
    updateEntity(m_pRootEntity, timeInfo);
}

void onut::SceneManager::updateEntity(Entity* pEntity, const onut::TimeInfo& timeInfo)
{
    m_pRootEntity->visit([](Entity* pEntity)
    {
        pEntity->onCreate();
        return true;
    });
    m_pRootEntity->visit([&timeInfo](Entity* pEntity)
    {
        if (pEntity->getEnabled() && pEntity->getVisible())
        {
            pEntity->onUpdate(timeInfo);
            return true;
        }
        return false;
    });
}

void onut::SceneManager::removeWeakReferences(Entity* pToDereference)
{
    m_pRootEntity->visit([pToDereference](Entity* pEntity)
    {
        pEntity->removeWeakReferences(pToDereference);
        return true;
    });
}

void onut::SceneManager::removeNonPersist(Entity* pEntity)
{
    m_pRootEntity->visit([this](Entity* pEntity)
    {
        if (pEntity->getPersist())
        {
            return false;
        }
        removeWeakReferences(pEntity);
        return true;
    });
    m_pRootEntity->visit([this](Entity* pEntity)
    {
        if (!pEntity->getPersist())
        {
            pEntity->removeFromParent();
            return false;
        }
        return true;
    });
}

void onut::SceneManager::loadScene(const std::string& filename)
{
    auto pScene = new onut::Entity();
    pScene->retain();
    pScene->loadPropertiesFromFile(filename);
    removeNonPersist(m_pRootEntity);
    m_pRootEntity->add(pScene);
}

void onut::SceneManager::loadSceneAsync(const std::string& filename)
{
    auto pDispatcher = onut::Game::getGame()->getComponent<Dispatcher>();
    m_pRootEntity->retain();
    pDispatcher->retain();
    std::async(std::launch::async, [=]()
    {
        auto pScene = new onut::Entity();
        pScene->retain();
        pScene->loadPropertiesFromFile(filename);
        pDispatcher->dispatch([=]
        {
            removeNonPersist(m_pRootEntity);
            m_pRootEntity->add(pScene);

            pScene->release();
            m_pRootEntity->release();
            pDispatcher->release();
        });
    });
}

onut::Entity* onut::SceneManager::getRootEntity() const
{
    return m_pRootEntity;
}
