#include "ContentManager.h"
#include "Resource.h"

onut::ContentManager::~ContentManager()
{
    for (auto kv : m_resources)
    {
        for (auto i = 0; i < kv.second.refCount; ++i)
        {
            kv.second.pResource->release();
        }
    }
}

bool onut::ContentManager::addResource(const std::string& name, Resource* pResource)
{
    if (!pResource) return false;
    auto it = m_resources.find(name);
    if (it != m_resources.end())
    {
        if (it->second.pResource != pResource) return false;
        ++it->second.refCount;
        pResource->retain();
        return true;
    }
    auto &res = m_resources[name];
    ++res.refCount;
    pResource->retain();
    res.pResource = pResource;
    return true;
}

bool onut::ContentManager::removeResource(Resource* pResource)
{
    if (!pResource) return false;
    for (auto it = m_resources.begin(); it != m_resources.end(); ++it)
    {
        if (it->second.pResource == pResource)
        {
            --it->second.refCount;
            pResource->release();
            if (it->second.refCount <= 0)
            {
                m_resources.erase(it);
            }
            return true;
        }
    }
    return false;
}

bool onut::ContentManager::removeResource(const std::string& name)
{
    auto it = m_resources.find(name);
    if (it == m_resources.end()) return false;
    --it->second.refCount;
    it->second.pResource->release();
    if (it->second.refCount <= 0)
    {
        m_resources.erase(it);
    }
    return true;
}
