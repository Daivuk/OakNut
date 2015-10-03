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

bool onut::Component::isEnabled() const
{
    return m_enabled;
}

bool onut::Component::isVisible() const
{
    return m_visible;
}

void onut::Component::setEnabled(bool enabled)
{
    m_enabled = enabled;
}

void onut::Component::setVisible(bool visible)
{
    m_visible = visible;
}
