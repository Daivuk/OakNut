#pragma once
#include "ObjectVector.h"
#include "PropertyManager.h"
#include "TimeInfo.h"

namespace onut
{
    class Component;

    class ComponentManager :
        public PropertyManager // It has serializable properties
    {
    public:
        ComponentManager();
        virtual ~ComponentManager();

        // Component events
        virtual void onCreate();
        virtual void onUpdate(const onut::TimeInfo& timeInfo);
        virtual void onDraw();

        // Insertion of components
        bool addComponent(Component* pComponent);
        bool insertComponentAtBeginning(Component* pComponent);
        bool insertComponentBefore(Component* pBefore, Component* pComponent);
        bool insertComponentAfter(Component* pAfter, Component* pComponent);
        template<typename Tcomponent> bool insertComponentBefore(Component* pComponent);
        template<typename Tcomponent> bool insertComponentAfter(Component* pComponent);
        bool removeComponent(Component* pComponent);

        // Removal of components
        template<typename Tcomponent> bool removeComponent();
        bool removeAllComponents();

        // Getters
        template<typename Tcomponent> Tcomponent* getComponent();
        bool hasComponent(Component* pComponent) const;

    private:
        PROPERTY_NOSETTER(ObjectVector<Component>, Components, ObjectVector<Component>());
    };

    template<typename Tcomponent> 
    inline bool insertComponentBefore(Component* pComponent)
    {
        auto pBefore = getComponent<Tcomponent>();
        if (!pBefore) return false;
        return insertComponentBefore(pBefore, pComponent);
    }

    template<typename Tcomponent> 
    inline bool insertComponentAfter(Component* pComponent)
    {
        auto pAfter = getComponent<Tcomponent>();
        if (!pAfter) return false;
        return insertComponentAfter(pAfter, pComponent);
    }

    template<typename Tcomponent>
    inline bool ComponentManager::removeComponent()
    {
        for (auto it = m_Components.begin(); it != m_Components.end(); ++it)
        {
            auto pTcomponent = dynamic_cast<Tcomponent*>(pComponent);
            if (pTcomponent)
            {
                m_Components.erase(it);
                pTcomponent->m_pComponentManager = nullptr;
                pTcomponent->release();
                return true;
            }
        }
        return false;
    }

    template<typename Tcomponent>
    inline Tcomponent* ComponentManager::getComponent()
    {
        for (auto pComponent : m_Components)
        {
            auto pTcomponent = dynamic_cast<Tcomponent*>(pComponent);
            if (pTcomponent)
            {
                return pTcomponent;
            }
        }
        return nullptr;
    }
}
