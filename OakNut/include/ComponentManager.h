#pragma once
#include <vector>

namespace onut
{
    class Component;

    class ComponentManager
    {
    public:
        ComponentManager();
        virtual ~ComponentManager();

        // Component events
        void onCreate();
        void onUpdate();
        void onDraw();

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
        template<typename Tcomponent> Tcomponent* getComponent() const;
        const std::vector<Component*> &getComponents() const;
        bool hasComponent(Component* pComponent);

    private:
        std::vector<Component*> m_components;
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
        for (auto it = m_components.begin(); it != m_components.end(); ++it)
        {
            auto pTcomponent = dynamic_cast<Tcomponent*>(pComponent);
            if (pTcomponent)
            {
                m_components.erase(it);
                pTcomponent->m_pComponentManager = nullptr;
                pTcomponent->release();
                return true;
            }
        }
        return false;
    }

    template<typename Tcomponent>
    inline Tcomponent* ComponentManager::getComponent() const
    {
        for (auto pComponent : m_components)
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