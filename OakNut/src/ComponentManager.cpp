#include "Component.h"
#include "ComponentManager.h"

#include <typeinfo>

onut::ComponentManager::ComponentManager()
{
}

onut::ComponentManager::~ComponentManager()
{
    for (auto pComponent : m_components)
    {
        pComponent->m_pComponentManager = nullptr;
        pComponent->release();
    }
}

bool onut::ComponentManager::addComponent(Component* pComponent)
{
    if (hasComponent(pComponent)) return false;
    pComponent->retain();
    pComponent->m_pComponentManager = this;
    m_components.push_back(pComponent);
    return true;
}

bool onut::ComponentManager::insertComponentAtBeginning(Component* pComponent)
{
    if (hasComponent(pComponent)) return false;
    pComponent->retain();
    pComponent->m_pComponentManager = this;
    m_components.insert(m_components.begin(), pComponent);
    return true;
}

bool onut::ComponentManager::insertComponentBefore(Component* pBefore, Component* pComponent)
{
    if (hasComponent(pComponent)) return false;
    for (auto it = m_components.begin(); it != m_components.end(); ++it)
    {
        auto pComponentOther = *it;
        if (pComponentOther == pBefore)
        {
            pComponent->retain();
            pComponent->m_pComponentManager = this;
            m_components.insert(it, pComponent);
            return true;
        }
    }
    return false;
}

bool onut::ComponentManager::insertComponentAfter(Component* pAfter, Component* pComponent)
{
    if (hasComponent(pComponent)) return false;
    for (auto it = m_components.begin(); it != m_components.end(); ++it)
    {
        auto pComponentOther = *it;
        if (pComponentOther == pAfter)
        {
            pComponent->retain();
            pComponent->m_pComponentManager = this;
            m_components.insert(it + 1, pComponent);
            return true;
        }
    }
    return false;
}

bool onut::ComponentManager::removeComponent(Component* pComponent)
{
    for (auto it = m_components.begin(); it != m_components.end(); ++it)
    {
        auto pComponentOther = *it;
        if (pComponentOther == pComponent)
        {
            m_components.erase(it);
            pComponent->m_pComponentManager = nullptr;
            pComponent->release();
            return true;
        }
    }
    return false;
}

bool onut::ComponentManager::removeAllComponents()
{
    bool bRemoved = !m_components.empty();
    for (auto pComponent : m_components)
    {
        pComponent->m_pComponentManager = nullptr;
        pComponent->release();
    }
    m_components.clear();
    return bRemoved;
}

bool onut::ComponentManager::hasComponent(Component* pComponent)
{
    for (auto pComponentOther : m_components)
    {
        if (typeid(*pComponentOther) == typeid(*pComponent)) return true;
    }
    return false;
}

const std::vector<onut::Component*> &onut::ComponentManager::getComponents() const
{
    return m_components;
}

void onut::ComponentManager::onCreate()
{
    for (auto pComponent : m_components)
    {
        if (!pComponent->isCreated())
        {
            pComponent->onCreate();
            pComponent->m_created = true;
        }
    }
}

void onut::ComponentManager::onUpdate()
{
    for (auto pComponent : m_components)
    {
        if (pComponent->isEnabled() && pComponent->isCreated())
        {
            pComponent->onUpdate();
        }
    }
}

void onut::ComponentManager::onDraw()
{
    for (auto pComponent : m_components)
    {
        if (pComponent->isVisible() && pComponent->isCreated())
        {
            pComponent->onDraw();
        }
    }
}
