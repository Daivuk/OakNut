#pragma once
#include "Component.h"

#include <unordered_map>

namespace onut
{
    class Resource;

    class ContentManager : public Component
    {
    public:
        virtual ~ContentManager();

        bool addResource(const std::string& name, Resource* pResource);
        bool removeResource(Resource* pResource);
        bool removeResource(const std::string& name);
        
        template<typename Ttype = Resource>
        Ttype* getResource(const std::string& name)
        {
            auto it = m_resources.find(name);
            if (it == m_resources.end()) return nullptr;
            return dynamic_cast<Ttype>(it->second.pResource);
        }

    private:
        struct sResource
        {
            Resource* pResource = nullptr;
            int refCount = 0;
        };
        std::unordered_map<std::string, sResource> m_resources;
    };
}
