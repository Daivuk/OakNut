#pragma once
#include "Object.h"

namespace onut
{
    class Component : public Object
    {
    public:
        Component();
        virtual ~Component();

        bool isCreated() const;
        bool isEnabled() const;
        bool isVisible() const;
        void setEnabled(bool enabled);
        void setVisible(bool visible);

        // Component events
        virtual void onCreate() {}
        virtual void onUpdate() {}
        virtual void onDraw() {}

    private:
        friend class ComponentManager;

        bool m_created = false;
        bool m_enabled = true;
        bool m_visible = true;
        ComponentManager* m_pComponentManager = nullptr;
    };
}
