#include "Entity.h"
#include "glm/gtx/transform.hpp"

onut::Entity::~Entity()
{
}

onut::Entity::Entity(const Entity& other)
{
}

onut::Entity* onut::Entity::getParent() const
{
    return m_pParent;
}

const glm::mat4& onut::Entity::getLocalMatrix() const
{
    return
        glm::scale(getScale()) *
        glm::translate(getPosition()) *
        glm::rotate(glm::radians(getRotation().z), glm::vec3(0, 0, 1)) *
        glm::rotate(glm::radians(getRotation().y), glm::vec3(0, 1, 0)) *
        glm::rotate(glm::radians(getRotation().x), glm::vec3(1, 0, 0));
}

const glm::mat4& onut::Entity::getWorldMatrix()
{
    if (m_isMatrixDirty)
    {
        m_worldMatrix = getLocalMatrix();
        if (m_pParent)
        {
            m_worldMatrix = m_pParent->getWorldMatrix() * m_worldMatrix;
        }
        m_isMatrixDirty = false;
    }
    return m_worldMatrix;
}

bool onut::Entity::add(Entity* pEntity)
{
    if (m_Children.contains(pEntity)) return false;
    m_Children.push_back(pEntity);
    pEntity->m_pParent = this;
    return true;
}

bool onut::Entity::remove(Entity* pEntity)
{
    if (!pEntity) return false;
    if (pEntity->m_pParent != this) return false;
    pEntity->m_pParent = nullptr;
    return m_Children.release(pEntity);
}

bool onut::Entity::removeFromParent()
{
    if (!m_pParent) return false;
    return m_pParent->remove(this);
}

void onut::Entity::setMatrixDirty(Entity* pEntity)
{
    pEntity->visit([](Entity* pEntity)
    {
        pEntity->m_isMatrixDirty = true;
        return true;
    });
}

void onut::Entity::onCreate()
{
    retain();
    ComponentManager::onCreate();
    release();
}

void onut::Entity::onUpdate(const onut::TimeInfo& timeInfo)
{
    retain();
    ComponentManager::onUpdate(timeInfo);
    release();
}

void onut::Entity::onDraw()
{
    retain();
    ComponentManager::onDraw();
    release();
}
