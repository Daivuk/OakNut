#include "Component.h"
#include "ComponentManager.h"

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
