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
        glm::rotate(getRotation().x, glm::vec3(1, 0, 0)) *
        glm::rotate(getRotation().y, glm::vec3(0, 1, 0)) *
        glm::rotate(getRotation().z, glm::vec3(0, 0, 1)) *
        glm::scale(getScale()) *
        glm::translate(getPosition());
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
