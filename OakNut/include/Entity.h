#pragma once
#include "ComponentManager.h"
#include "glm/glm.hpp"
#include "Object.h"

namespace onut
{
    class Entity : 
        public ComponentManager, // It has components
        public onut::Object // It has ref counting
    {
    public:
        virtual ~Entity();

        const glm::mat4& getLocalMatrix() const;
        const glm::mat4& getWorldMatrix();

        Entity* getParent() const;
        bool isCreated() const;

        bool add(Entity* pEntity);
        bool remove(Entity* pEntity);

        void onCreate();
        void onUpdate();
        void onDraw();

    private:
        PROPERTY_DIRTY(glm::vec3, Position, glm::vec3(0, 0, 0), m_isMatrixDirty);
        PROPERTY_DIRTY(glm::vec3, Rotation, glm::vec3(0, 0, 0), m_isMatrixDirty);
        PROPERTY_DIRTY(glm::vec3, Scale, glm::vec3(1, 1, 1), m_isMatrixDirty);
        PROPERTY_NOSETTER(std::vector<Entity*>, Children, std::vector<Entity*>());
        PROPERTY(std::string, Name, "Entity");

    private:
        glm::mat4 m_worldMatrix;
        bool m_isMatrixDirty = true;
        Entity* m_pParent = nullptr;
    };
}
