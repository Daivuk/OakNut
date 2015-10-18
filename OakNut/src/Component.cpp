#include "Component.h"
#include "ComponentManager.h"
#include "Entity.h"

onut::Component::Component()
{
}

onut::Component::~Component()
{
    if (m_pComponentManager)
    {
        m_pComponentManager->removeComponent(this);
    }
}

bool onut::Component::isCreated() const
{
    return m_created;
}

onut::ComponentManager* onut::Component::getComponentManager() const
{
    return m_pComponentManager;
}

onut::Entity* onut::Component::getEntity() const
{
    return dynamic_cast<Entity*>(m_pComponentManager);
}

onut::Entity* onut::Component::getParent() const
{
    auto pEntity = getEntity();
    if (!pEntity) return nullptr;
    return pEntity->getParent();
}
