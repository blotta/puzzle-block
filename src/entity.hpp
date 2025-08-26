#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "vmath.hpp"
#include <algorithm>
#include <memory>
#include <unordered_map>
#include <vector>

class Entity;

class Component
{
  public:
    Entity* owner = nullptr;
    virtual ~Component() = default;

    virtual void init();
    virtual void update(float dt);
    virtual void draw();
    virtual void drawGUI();
    virtual void onEnable();
    virtual void onDisable();
    virtual void onDestroy();
};


////////////////////////
// Builtin Components //
////////////////////////
class CTransform : public Component
{
  public:
    vec2f pos;
    CTransform();
    CTransform(const vec2f& p);
};


////////////
// ENTITY //
////////////

class Entity
{
    friend class EntityManager;
    std::vector<std::unique_ptr<Component>> components;
    bool active = true;
    bool toDelete = false;
    size_t mId = 0;
    std::string mTag = "default";

  public:
    Entity(size_t id);
    Entity(size_t id, const std::string& tag);

    template <typename T, typename... Args> T* addComponent(Args&&... args)
    {
        auto comp = std::make_unique<T>(std::forward<Args>(args)...);
        comp->owner = this;
        components.push_back(std::move(comp));
        T* ptr = static_cast<T*>(components.back().get());
        return ptr;
    }

    template <typename T> T* getComponent()
    {
        for (auto& c : components)
        {
            if (auto ptr = dynamic_cast<T*>(c.get()))
                return ptr;
        }
        return nullptr;
    }

    size_t id() const;
    const std::string& tag() const;
    void init();
    void update(float dt);
    void draw();
    void drawGUI();
    void setActive(bool state);
    void destroy();
    bool isActive() const;
};

////////////////////
// ENTITY MANAGER //
////////////////////

class EntityManager
{
    std::vector<std::shared_ptr<Entity>> mEntities;
    std::unordered_map<std::string, std::vector<std::shared_ptr<Entity>>> mEntityMap;
    std::vector<std::shared_ptr<Entity>> mEntitiesToAdd;
    size_t mTotalEntities = 0;

  public:
    std::shared_ptr<Entity> add();
    std::shared_ptr<Entity> add(const std::string& tag);
    std::vector<std::shared_ptr<Entity>>& get();
    std::vector<std::shared_ptr<Entity>>& get(const std::string& tag);
    void init();
    void removeAndAddEntities(bool initEntities);
    void update(float dt);
    void draw();
    void drawGUI();
};

#endif