#pragma once

#include "entity.h"

namespace XEngine
{
    class EntityManager
    {
    public:
        void addEnitity(std::string name, Entity *ent);
        Entity* getEntitybyName(std::string name);
        inline int getCount() const { return count; }

    private:
        EntityManager() {};
        EntityManager(const EntityManager&);
        EntityManager& operator=(EntityManager&);

    private:
        std::map<std::string, Entity*> mEntities;
        int count;

    public:
        static EntityManager& getInstance()
        {
            static EntityManager instance;
            return instance;
        }

    };
}
