#pragma once
#include "Component.h"
#include "glm/glm.hpp"

#include <vector>

namespace onut
{
    class Entity;

    class SceneNode : public Component
    {
    public:
        const glm::mat4& getLocalMatrix() const;
        const glm::mat4& getWorldMatrix();

        Entity* getEntity() const;

        bool add(Entity* pEntity);
        bool remove(Entity* pEntity);

        void onCreate();
        void onUpdate();
        void onDraw();

    private:
        PROPERTY_DIRTY(glm::vec3, Position, glm::vec3(0, 0, 0), m_isDirty);
        PROPERTY_DIRTY(glm::vec3, Rotation, glm::vec3(0, 0, 0), m_isDirty);
        PROPERTY_DIRTY(glm::vec3, Scale, glm::vec3(1, 1, 1), m_isDirty);
        PROPERTY(std::vector<Entity*>, Children, std::vector<Entity*>());

    private:
        glm::mat4 m_worldMatrix;
        bool m_isDirty = true;
        SceneNode* m_pParent;
        Entity* m_pEntity = nullptr;
    };
}
