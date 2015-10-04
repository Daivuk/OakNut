#include "Dispatcher.h"
#include "Game.h"
#include "SceneManager.h"
#include "SceneNode.h"

#include <future>

onut::SceneManager::SceneManager()
{
    m_pRoot = new onut::SceneNode();
    m_pRoot->retain();
}

onut::SceneManager::~SceneManager()
{
    m_pRoot->release();
}

void onut::SceneManager::onCreate()
{
    m_pRoot->onCreate();

    // Load the default scene
    if (!getStartupScene().empty())
    {
        loadScene(getStartupScene());
    }
}

void onut::SceneManager::onUpdate()
{
    m_pRoot->onUpdate();
}

void onut::SceneManager::onDraw()
{
    m_pRoot->onDraw();
}

void onut::SceneManager::loadScene(const std::string& filename)
{
    auto pDispatcher = onut::Game::getGame()->getComponent<Dispatcher>();
    m_pRoot->retain();
    pDispatcher->retain();
    std::async(std::launch::async, [=]()
    {
        auto pScene = new onut::Entity();
        pScene->retain();
        pScene->loadPropertiesFromFile(filename);
        pDispatcher->dispatch([=]
        {
            m_pRoot->add(pScene);

            pScene->release();
            m_pRoot->release();
            pDispatcher->release();
        });
    });
}
