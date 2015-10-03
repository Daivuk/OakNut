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
    auto pDispatcher = onut::Game::getMainDispatcher();
    m_pRoot->retain();
    pDispatcher->retain();
    std::async(std::launch::async, [=](decltype(m_pRoot) pRoot)
    {
        auto pScene = new onut::Entity();
        auto pSceneNode = new onut::SceneNode();
        pScene->addComponent(pSceneNode);
        pScene->retain();
        pSceneNode->retain();
        pScene->loadPropertiesFromFile(filename);
        pDispatcher->dispatch([=]
        {
            pRoot->add(pScene);

            pSceneNode->release();
            pScene->release();
            pRoot->release();
            pDispatcher->release();
        });
    }, m_pRoot);
}
