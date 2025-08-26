#include "entity.hpp"

Entity::Entity(size_t id) : mId(id)
{
}

Entity::Entity(size_t id, const std::string& tag) : mId(id), mTag(tag)
{
}

size_t Entity::id() const
{
    return mId;
}

const std::string& Entity::tag() const
{
    return mTag;
}

void Entity::init()
{
    for (auto& c : components)
        c->init();
    for (auto& c : components)
        c->onEnable();
}

void Entity::update(float dt)
{
    if (!active)
        return;
    for (auto& c : components)
        c->update(dt);
}

void Entity::draw()
{
    if (!active)
        return;
    for (auto& c : components)
        c->draw();
}

void Entity::drawGUI()
{
    if (!active)
        return;
    for (auto& c : components)
        c->drawGUI();
}

void Entity::setActive(bool state)
{
    if (active == state)
        return;
    active = state;
    for (auto& c : components)
    {
        if (state)
            c->onEnable();
        else
            c->onDisable();
    }
}

void Entity::destroy()
{
    for (auto& c : components)
        c->onDestroy();
    components.clear();
    active = false;
    toDelete = true;
}

bool Entity::isActive() const
{
    return active;
}

std::shared_ptr<Entity> EntityManager::add()
{
    auto e = std::make_shared<Entity>(mTotalEntities++);
    mEntitiesToAdd.push_back(e);
    return e;
}

std::shared_ptr<Entity> EntityManager::add(const std::string& tag)
{
    auto e = std::make_shared<Entity>(mTotalEntities++, tag);
    mEntitiesToAdd.push_back(e);
    return e;
}

std::vector<std::shared_ptr<Entity>>& EntityManager::get()
{
    return mEntities;
}

std::vector<std::shared_ptr<Entity>>& EntityManager::get(const std::string& tag)
{
    return mEntityMap[tag];
}

void EntityManager::init()
{
    for (auto& e : mEntities)
        e->init();
}

void EntityManager::removeAndAddEntities(bool initEntities)
{
    // remove deleted entities
    mEntities.erase(std::remove_if(mEntities.begin(), mEntities.end(),
                                   [&](const std::shared_ptr<Entity>& e) {
                                       if (e->toDelete)
                                       {
                                           auto& vec = mEntityMap[e->tag()];
                                           vec.erase(std::remove(vec.begin(), vec.end(), e), vec.end());
                                           return true;
                                       }
                                       return false;
                                   }),
                    mEntities.end());

    // add new entities
    for (auto e : mEntitiesToAdd)
    {
        mEntities.push_back(e);
        mEntityMap[e->tag()].push_back(e);
        if (initEntities)
            e->init();
    }
    mEntitiesToAdd.clear();
}

void EntityManager::update(float dt)
{
    this->removeAndAddEntities(true);

    // update entities
    for (auto& e : mEntities)
        if (e->isActive())
            e->update(dt);
}

void EntityManager::draw()
{
    for (auto& e : mEntities)
        if (e->isActive())
            e->draw();
}

void EntityManager::drawGUI()
{
    for (auto& e : mEntities)
        if (e->isActive())
            e->drawGUI();
}