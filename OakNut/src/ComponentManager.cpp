#include "Component.h"
#include "ComponentManager.h"

#include <typeinfo>

onut::ComponentManager::ComponentManager()
{
}

onut::ComponentManager::~ComponentManager()
{
    for (auto pComponent : m_Components)
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
    m_Components.push_back(pComponent);
    return true;
}

bool onut::ComponentManager::insertComponentAtBeginning(Component* pComponent)
{
    if (hasComponent(pComponent)) return false;
    pComponent->retain();
    pComponent->m_pComponentManager = this;
    m_Components.insert(m_Components.begin(), pComponent);
    return true;
}

bool onut::ComponentManager::insertComponentBefore(Component* pBefore, Component* pComponent)
{
    if (hasComponent(pComponent)) return false;
    for (auto it = m_Components.begin(); it != m_Components.end(); ++it)
    {
        auto pComponentOther = *it;
        if (pComponentOther == pBefore)
        {
            pComponent->retain();
            pComponent->m_pComponentManager = this;
            m_Components.insert(it, pComponent);
            return true;
        }
    }
    return false;
}

bool onut::ComponentManager::insertComponentAfter(Component* pAfter, Component* pComponent)
{
    if (hasComponent(pComponent)) return false;
    for (auto it = m_Components.begin(); it != m_Components.end(); ++it)
    {
        auto pComponentOther = *it;
        if (pComponentOther == pAfter)
        {
            pComponent->retain();
            pComponent->m_pComponentManager = this;
            m_Components.insert(it + 1, pComponent);
            return true;
        }
    }
    return false;
}

bool onut::ComponentManager::removeComponent(Component* pComponent)
{
    for (auto it = m_Components.begin(); it != m_Components.end(); ++it)
    {
        auto pComponentOther = *it;
        if (pComponentOther == pComponent)
        {
            m_Components.erase(it);
            pComponent->m_pComponentManager = nullptr;
            pComponent->release();
            return true;
        }
    }
    return false;
}

bool onut::ComponentManager::removeAllComponents()
{
    bool bRemoved = !m_Components.empty();
    for (auto pComponent : m_Components)
    {
        pComponent->m_pComponentManager = nullptr;
        pComponent->release();
    }
    m_Components.clear();
    return bRemoved;
}

bool onut::ComponentManager::hasComponent(Component* pComponent)
{
    for (auto pComponentOther : m_Components)
    {
        if (typeid(*pComponentOther) == typeid(*pComponent)) return true;
    }
    return false;
}

void onut::ComponentManager::onCreate()
{
    for (auto pComponent : m_Components)
    {
        if (!pComponent->isCreated())
        {
            pComponent->onCreate();
            pComponent->m_created = true;
        }
    }
}

void onut::ComponentManager::onUpdate(const onut::TimeInfo& timeInfo)
{
    for (auto pComponent : m_Components)
    {
        if (pComponent->getEnabled() && pComponent->isCreated())
        {
            pComponent->onUpdate(timeInfo);
        }
    }
}

void onut::ComponentManager::onDraw()
{
    for (auto pComponent : m_Components)
    {
        if (pComponent->getVisible() && pComponent->isCreated())
        {
            pComponent->onDraw();
        }
    }
}
