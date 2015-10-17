#pragma once
#include "Object.h"
#include "PropertyManager.h"
#include "TimeInfo.h"

namespace onut
{
    class ComponentManager;
    class Entity;

    class Component : 
        public PropertyManager, // It has serializable properties
        public Object // It has ref counting
    {
    public:
        Component();
        virtual ~Component();

        virtual bool isCreated() const;
        ComponentManager* getComponentManager() const;
        Entity* getEntity() const;

        // Component events
        virtual void onCreate() {}
        virtual void onUpdate(const onut::TimeInfo& timeInfo) {}
        virtual void onDraw() {}

        template<typename Tcomponent> Tcomponent* getComponent();

    private:
        friend class ComponentManager;

        // Common component properties
        PROPERTY(bool, Enabled, true);
        PROPERTY(bool, Visible, true);

        bool m_created = false;
        ComponentManager* m_pComponentManager = nullptr;
    };

    template<typename Tcomponent>
    inline Tcomponent* Component::getComponent()
    {
        if (!m_pComponentManager) return nullptr;
        return m_pComponentManager->getComponent<Tcomponent>();
    }
}
