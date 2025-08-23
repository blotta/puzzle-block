#include "scene_unity.hpp"
#include "game.hpp"
#include "input_manager.hpp"
#include "log.hpp"

class CTransform : public Component
{
  public:
    vec2f pos;
    CTransform(const vec2f& p) : pos(p)
    {
    }
};

class CShape : public Component
{
  public:
    CShape()
    {
    }

    CTransform* transform = nullptr;

    void init() override
    {
        transform = owner->getComponent<CTransform>();
    }

    void draw() override
    {
        Game::DrawLine(transform->pos.x - 10, transform->pos.y, transform->pos.x + 10, transform->pos.y, {255, 255, 255, 255});
        Game::DrawPoint(transform->pos.x-10, transform->pos.y, {0, 255, 0, 255});
        Game::DrawPoint(transform->pos.x+10, transform->pos.y, {0, 255, 0, 255});
    }
};

void UnityScene::init()
{
    auto e = entities.add();
    auto transformComponent = e->addComponent<CTransform>(vec2f{20, 20});
    auto shapeComponent = e->addComponent<CShape>();
}

void UnityScene::update(float dt)
{
    if (Input::MouseJustPressed(SDL_BUTTON_LEFT))
    {
        vec2 mousePos;
        Input::MousePosition(&mousePos.x, &mousePos.y);
        auto e = entities.add("up");
        auto transformComponent = e->addComponent<CTransform>(vec2f{mousePos.x, mousePos.y});
        auto shapeComponent = e->addComponent<CShape>();
    }
    if (Input::JustPressed(SDL_SCANCODE_X))
    {
        // delete entity
        if (!entities.get("up").empty())
        {
            auto e = entities.get("up").back();
            e->destroy();
        }
    }
}

void UnityScene::draw()
{
}

void UnityScene::dispose()
{
}