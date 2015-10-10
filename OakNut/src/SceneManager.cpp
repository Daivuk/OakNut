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
    // Load the default scene
    if (!getStartupScene().empty())
    {
        loadScene(getStartupScene());
    }
}

void onut::SceneManager::onUpdate()
{
    createEntity(m_pRootEntity);
    updateEntity(m_pRootEntity);
}

void onut::SceneManager::createEntity(Entity* pEntity)
{
    pEntity->onCreate();
    for (auto pChild : pEntity->getChildren())
    {
        createEntity(pChild);
    }
}

void onut::SceneManager::updateEntity(Entity* pEntity)
{
    pEntity->onUpdate();
    for (auto pChild : pEntity->getChildren())
    {
        if (pChild->getEnabled() &&
            pChild->getVisible())
        {
            updateEntity(pChild);
        }
    }
}

void onut::SceneManager::onDraw()
{
    drawEntity(m_pRootEntity);
}

void onut::SceneManager::drawEntity(Entity* pEntity)
{
    pEntity->onDraw();
    for (auto pChild : pEntity->getChildren())
    {
        if (pChild->getVisible())
        {
            drawEntity(pChild);
        }
    }
}

void onut::SceneManager::removeNonPersist(Entity* pEntity)
{
    //auto children = pEntity->getChildren();
    //for (auto pChild : children)
    //{
    //    if (!pChild->getPersist())
    //    {
    //        Entity::destroy(
    //    }
    //}
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
