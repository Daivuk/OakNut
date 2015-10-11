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
        glm::translate(getPosition()) *
        glm::rotate(glm::radians(getRotation().z), glm::vec3(0, 0, 1)) *
        glm::rotate(glm::radians(getRotation().y), glm::vec3(0, 1, 0)) *
        glm::rotate(glm::radians(getRotation().x), glm::vec3(1, 0, 0)) *
        glm::scale(getScale());
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
    for (auto &pEntityOther : m_Children)
    {
        if (pEntityOther == pEntity) return false;
    }
    pEntity->retain();
    m_Children.push_back(pEntity);
    return true;
}

bool onut::Entity::remove(Entity* pEntity)
{
    for (auto it = m_Children.begin(); it != m_Children.end(); ++it)
    {
        if (*it == pEntity)
        {
            m_Children.erase(it);
            pEntity->release();
            return true;
        }
    }
    return false;
}

void onut::Entity::setMatrixDirty(Entity* pEntity)
{
    pEntity->m_isMatrixDirty = true;
    for (auto it = pEntity->m_Children.begin(); it != pEntity->m_Children.end(); ++it)
    {
        setMatrixDirty(*it);
    }
}
