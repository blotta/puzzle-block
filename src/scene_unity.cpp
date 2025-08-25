#include "scene_unity.hpp"
#include "game.hpp"
#include "input_manager.hpp"
#include "log.hpp"
#include "util.hpp"
#include <format>

class CShape : public Component
{
  public:
    CShape()
    {
    }

    CTransform* transform = nullptr;
    SpriteID spr = SPR_FLOOR;

    void init() override
    {
        transform = owner->getComponent<CTransform>();
        spr = randomFrom({SPR_FLOOR, SPR_FLOOR, SPR_FLOOR_HIGHLIGHT, SPR_FLOOR_HIGHLIGHT, SPR_FLOOR_THIN,
                          SPR_FLOOR_FINISH, SPR_FLOOR_START, SPR_FLOOR_HIGHLIGHT});
    }

    void draw() override
    {
        Game::DrawSprite(transform->pos.x, transform->pos.y, spr);
    }
};

class CMovement : public Component
{
  public:
    CMovement()
    {
    }

    vec2f velocity;
    float minSpeed = 40.f;
    float maxSpeed = 70.f;
    CTransform* transform = nullptr;

    void init() override
    {
        transform = owner->getComponent<CTransform>();
        velocity = {randomNeg1to1(), randomNeg1to1()};
        float speed = minSpeed + random01() * (maxSpeed - minSpeed);
        velocity = velocity.normalized() * speed;
        // Log::debug("%f, %f", velocity.x, velocity.y);
    }

    void update(float dt) override
    {
        auto pos = transform->pos + velocity * dt;
        if (pos.x < 0)
        {
            pos.x = 0;
            velocity.x = -velocity.x;
        }
        if (pos.x > Game::ScreenWidth())
        {
            pos.x = Game::ScreenWidth();
            velocity.x = -velocity.x;
        }
        if (pos.y < 0)
        {
            pos.y = 0;
            velocity.y = -velocity.y;
        }
        if (pos.y > Game::ScreenHeight())
        {
            pos.y = Game::ScreenHeight();
            velocity.y = -velocity.y;
        }

        transform->pos = pos;
    }
};

void UnityScene::init()
{
    entities.get().reserve(200000);

    auto e = entities.add();
    auto transformComponent = e->addComponent<CTransform>(vec2f{20, 20});
    auto shapeComponent = e->addComponent<CShape>();
    auto moveComponent = e->addComponent<CMovement>();
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
        auto moveComponent = e->addComponent<CMovement>();
    }
    if (Input::MouseJustPressed(SDL_BUTTON_RIGHT))
    {
        for (int i = 0; i < 10000; i++)
        {
            vec2 mousePos;
            Input::MousePosition(&mousePos.x, &mousePos.y);
            auto e = entities.add("up");
            auto transformComponent = e->addComponent<CTransform>(vec2f{mousePos.x, mousePos.y});
            auto shapeComponent = e->addComponent<CShape>();
            auto moveComponent = e->addComponent<CMovement>();
        }
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

    Game::Text(20, Game::ScreenHeight() - 40, std::format("entities {}", entities.get().size()),
               {.valign = VerticalAlign::BOTTOM});
    Game::Text(20, Game::ScreenHeight() - 20, std::format("FPS {}", Game::FPS()), {.valign = VerticalAlign::BOTTOM});
}

void UnityScene::dispose()
{
}