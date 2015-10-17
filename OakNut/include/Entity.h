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
        Entity() {}
        Entity(const Entity& other);
        virtual ~Entity();

        virtual void onCreate();
        virtual void onUpdate(const onut::TimeInfo& timeInfo);
        virtual void onDraw();

        const glm::mat4& getLocalMatrix() const;
        const glm::mat4& getWorldMatrix();

        Entity* getParent() const;
        bool isCreated() const;

        bool add(Entity* pEntity);
        bool remove(Entity* pEntity);
        bool removeFromParent();

        template<typename TLambda, typename ... TArgs>
        void visit(const TLambda& lambda, TArgs ... args)
        {
            if (lambda(this, args...))
            {
                for (decltype(m_Children.size()) i = 0; i < m_Children.size(); ++i)
                {
                    auto pChild = m_Children[i];
                    if (pChild)
                    {
                        pChild->visit(lambda, args...);
                    }
                }
            }
        }

    private:
        PROPERTY(bool, Enabled, true);
        PROPERTY(bool, Visible, true);
        PROPERTY_DIRTY(glm::vec3, Position, glm::vec3(0, 0, 0), setMatrixDirty);
        PROPERTY_DIRTY(glm::vec3, Rotation, glm::vec3(0, 0, 0), setMatrixDirty);
        PROPERTY_DIRTY(glm::vec3, Scale, glm::vec3(1, 1, 1), setMatrixDirty);
        PROPERTY_NOSETTER(onut::ObjectVector<Entity>, Children, onut::ObjectVector<Entity>());
        PROPERTY(std::string, Name, "Entity");
        PROPERTY(bool, Persist, false);

    private:
        static void setMatrixDirty(Entity* pEntity);

        glm::mat4 m_worldMatrix;
        bool m_isMatrixDirty = true;
        Entity* m_pParent = nullptr;
    };
}
