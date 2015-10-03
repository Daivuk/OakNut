#pragma once
#include "Object.h"
#include "PropertyManager.h"

namespace onut
{
    class ComponentManager;

    class Component : 
        public PropertyManager, // It has serializable properties
        public Object // It has ref counting
    {
    public:
        Component();
        virtual ~Component();

        bool isCreated() const;
        ComponentManager* getComponentManager() const;

        // Component events
        virtual void onCreate() {}
        virtual void onUpdate() {}
        virtual void onDraw() {}

    private:
        friend class ComponentManager;

        // Common component properties
        PROPERTY(bool, Enabled, true);
        PROPERTY(bool, Visible, true);

        bool m_created = false;
        ComponentManager* m_pComponentManager = nullptr;
    };
}
