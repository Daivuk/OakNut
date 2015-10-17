#pragma once
#include <vector>

namespace onut
{
    template<typename TObjectType>
    class ObjectVector
    {
    private:
        std::vector<TObjectType*> m_objects;

    public:
        ObjectVector()
        {
        }

        ObjectVector(const ObjectVector<TObjectType*>& other)
        {
            m_objects = other.m_objects;
            for (decltype(m_objects.size()) i = 0; i < m_objects.size(); ++i)
            {
                auto pObject = m_objects[i];
                if (pObject)
                {
                    m_objects[i]->retain();
                }
            }
        }

        ~ObjectVector()
        {
            for (auto pObject : m_objects)
            {
                if (pObject)
                {
                    pObject->release();
                }
            }
        }

        auto size() const -> decltype(m_objects.size())
        {
            return m_objects.size();
        }

        template<typename TInt>
        TObjectType* operator[](TInt index)
        {
            return m_objects[index];
        }

        void clear()
        {
            for (decltype(m_objects.size()) i = 0; i < m_objects.size(); ++i)
            {
                auto pObject = m_objects[i];
                if (pObject)
                {
                    m_objects[i]->release();
                }
            }
            m_objects.clear();
        }

        void push_back(TObjectType* pObject)
        {
            if (pObject) pObject->retain();
            m_objects.push_back(pObject);
        }

        bool empty() const
        {
            return m_objects.empty();
        }

        bool operator==(const ObjectVector<TObjectType*>& other) const
        {
            return m_objects == other.m_objects;
        }

        template<typename TInt>
        bool eraseAt(TInt index)
        {
            if (index < 0 || index >= static_cast<TInt>(m_objects.size())) return false;
            auto pObject = m_objects[index];
            if (pObject) pObject->release();
            m_objects.erase(m_objects.begin() + index);
            return true;
        }

        bool erase(TObjectType* pObject)
        {
            bool bFound = false;
            for (auto it = m_objects.begin(); it != m_objects.end();)
            {
                if (*it == pObject)
                {
                    it = m_objects.erase(it);
                    if (pObject) pObject->release();
                    bFound = true;
                }
                else
                {
                    ++it;
                }
            }
            return bFound;
        }

        bool cleanup()
        {
            return erase(nullptr);
        }

        bool release()
        {
            if (m_objects.empty()) return false;
            for (decltype(m_objects.size()) i = 0; i < m_objects.size(); ++i)
            {
                auto &pObjectIt = m_objects[i];
                if (pObjectIt)
                {
                    pObjectIt->release();
                    pObjectIt = nullptr;
                }
            }
            return true;
        }

        bool release(TObjectType* pObject)
        {
            bool bRet = false;
            for (decltype(m_objects.size()) i = 0; i < m_objects.size(); ++i)
            {
                auto &pObjectIt = m_objects[i];
                if (pObjectIt == pObject)
                {
                    pObjectIt->release();
                    pObjectIt = nullptr;
                    bRet = true;
                }
            }
            return bRet;
        }

        bool contains(TObjectType* pObject) const
        {
            for (decltype(m_objects.size()) i = 0; i < m_objects.size(); ++i)
            {
                if (m_objects[i] == pObject) return true;
            }
            return false;
        }

        auto indexOf(TObjectType* pObject) const -> decltype(m_objects.size())
        {
            decltype(m_objects.size()) i = 0;
            for (; i < m_objects.size(); ++i)
            {
                if (m_objects[i] == pObject) return i;
            }
            return i;
        }

        template<typename TInt>
        bool insert(TInt index, TObjectType* pObject)
        {
            if (index < 0 || index > static_cast<TInt>(m_objects.size())) return false;
            m_objects.insert(m_objects.begin() + index, pObject);
            return true;
        }

        bool insertBefore(TObjectType* pBefore, TObjectType* pObject)
        {
            auto index = indexOf(pBefore);
            return insert(index, pObject);
        }

        bool insertAfter(TObjectType* pBefore, TObjectType* pObject)
        {
            auto index = indexOf(pBefore);
            return insert(index + 1, pObject);
        }

        TObjectType** begin()
        {
            if (m_objects.empty()) return nullptr;
            return &m_objects[0];
        }

        TObjectType** end()
        {
            if (m_objects.empty()) return nullptr;
            return begin() + m_objects.size();
        }
    };
};
