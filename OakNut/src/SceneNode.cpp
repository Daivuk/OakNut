#include "Entity.h"
#include "SceneNode.h"
#include "glm/gtx/transform.hpp"

onut::Entity* onut::SceneNode::getEntity() const
{
    return dynamic_cast<onut::Entity*>(getComponentManager());
}

const glm::mat4& onut::SceneNode::getLocalMatrix() const
{
    return 
        glm::rotate(getRotation().x, glm::vec3(1, 0, 0)) *
        glm::rotate(getRotation().y, glm::vec3(0, 1, 0)) *
        glm::rotate(getRotation().z, glm::vec3(0, 0, 1)) *
        glm::scale(getScale()) *
        glm::translate(getPosition());
}

const glm::mat4& onut::SceneNode::getWorldMatrix()
{
    if (m_isDirty)
    {
        m_worldMatrix = getLocalMatrix();
        if (m_pParent)
        {
            m_worldMatrix = m_pParent->getWorldMatrix() * m_worldMatrix;
        }
        m_isDirty = false;
    }
    return m_worldMatrix;
}

bool onut::SceneNode::add(Entity* pEntity)
{
    pEntity->retain();
    m_Children.push_back(pEntity);
    return true;
}

bool onut::SceneNode::remove(Entity* pEntity)
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

void onut::SceneNode::onCreate()
{
    for (auto pChild : getChildren())
    {
        pChild->onCreate();
    }
}

void onut::SceneNode::onUpdate()
{
    for (auto pChild : getChildren())
    {
        pChild->onUpdate();
    }
}

void onut::SceneNode::onDraw()
{
    for (auto pChild : getChildren())
    {
        pChild->onDraw();
    }
}
