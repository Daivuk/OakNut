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
    m_pRootEntity->onCreate();
    m_pRootEntity->onUpdate();
}

void onut::SceneManager::onDraw()
{
    m_pRootEntity->onDraw();
}

void onut::SceneManager::loadScene(const std::string& filename)
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
            m_pRootEntity->add(pScene);

            pScene->release();
            m_pRootEntity->release();
            pDispatcher->release();
        });
    });
}
