#pragma once
#include <typeindex>

class CGameObject;

class CObjectPool
{
public:
    static CObjectPool& Get()
    {
        static CObjectPool instance;
        return instance;
    }

    template<typename T>
    void Push(T* obj)
    {
        m_pool[typeid(T)].push_back(obj);
    }

    template<typename T>
    T* Pop()
    {
        auto& pool = m_pool[typeid(T)];
        if (!pool.empty())
        {
            T* obj = static_cast<T*>(pool.back());
            pool.pop_back();
            return obj;
        }
        return nullptr;
    }

private:
    unordered_map<std::type_index, vector<CGameObject*>> m_pool;
};