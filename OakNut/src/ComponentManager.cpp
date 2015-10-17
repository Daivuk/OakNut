#include "Component.h"
#include "ComponentManager.h"

#include <typeinfo>

onut::ComponentManager::ComponentManager()
{
}

onut::ComponentManager::~ComponentManager()
{
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
    m_Components.insert(0, pComponent);
    return true;
}

bool onut::ComponentManager::insertComponentBefore(Component* pBefore, Component* pComponent)
{
    if (hasComponent(pComponent)) return false;
    auto ret = m_Components.insertBefore(pBefore, pComponent);
    if (ret)
    {
        pComponent->m_pComponentManager = this;
    }
    return ret;
}

bool onut::ComponentManager::insertComponentAfter(Component* pAfter, Component* pComponent)
{
    if (hasComponent(pComponent)) return false;
    auto ret = m_Components.insertAfter(pAfter, pComponent);
    if (ret)
    {
        pComponent->m_pComponentManager = this;
    }
    return ret;
}

bool onut::ComponentManager::removeComponent(Component* pComponent)
{
    if (!pComponent) return false;
    if (pComponent->m_pComponentManager != this) return false;
    pComponent->m_pComponentManager = nullptr;
    return m_Components.release(pComponent);
}

bool onut::ComponentManager::removeAllComponents()
{
    bool ret = false;
    for (decltype(m_Components.size()) i = 0; i < m_Components.size(); ++i)
    {
        auto pComponent = m_Components[i];
        if (pComponent)
        {
            pComponent->m_pComponentManager = nullptr;
            ret = true;
        }
    }
    m_Components.release();
    return ret;
}

bool onut::ComponentManager::hasComponent(Component* pComponent) const
{
    return m_Components.contains(pComponent);
}

void onut::ComponentManager::onCreate()
{
    for (decltype(m_Components.size()) i = 0; i < m_Components.size(); ++i)
    {
        auto pComponent = m_Components[i];
        if (pComponent)
        {
            if (!pComponent->isCreated())
            {
                pComponent->onCreate();
                pComponent->m_created = true;
            }
        }
    }
}

void onut::ComponentManager::onUpdate(const onut::TimeInfo& timeInfo)
{
    for (decltype(m_Components.size()) i = 0; i < m_Components.size(); ++i)
    {
        auto pComponent = m_Components[i];
        if (pComponent)
        {
            if (pComponent->getEnabled() && pComponent->isCreated())
            {
                pComponent->onUpdate(timeInfo);
            }
        }
    }
    m_Components.erase(nullptr);
}

void onut::ComponentManager::onDraw()
{
    for (decltype(m_Components.size()) i = 0; i < m_Components.size(); ++i)
    {
        auto pComponent = m_Components[i];
        if (pComponent)
        {
            if (pComponent->getVisible() && pComponent->isCreated())
            {
                pComponent->onDraw();
            }
        }
    }
}
