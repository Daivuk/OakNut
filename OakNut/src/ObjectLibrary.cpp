#include "ObjectLibrary.h"

std::unordered_map<std::string, onut::ObjectLibrary::IObjectFactory*> onut::ObjectLibrary::m_objectFactories;

onut::Object* onut::ObjectLibrary::createObject(const std::string& name)
{
    auto it = m_objectFactories.find(name);
    if (it == m_objectFactories.end()) return nullptr;
    return it->second->createObject();
}

void onut::ObjectLibrary::clear()
{
    for (auto kv : m_objectFactories)
    {
        delete kv.second;
    }
    m_objectFactories.clear();
}
