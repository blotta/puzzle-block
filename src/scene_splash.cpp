#include "scene_splash.hpp"
#include "game.hpp"
#include "input_manager.hpp"
#include "log.hpp"
#include "util.hpp"

void SplashScene::init()
{
    Log::info("Loading splash scene\n");

    Game::SetFontSize(32);
    int textWidth = Game::TextWidth("BLT GAMES");
    int textStart = Game::ScreenWidth() / 2 - textWidth / 2;

    splashAnim.duration = 3.0f;
    splashAnim.mode = AnimationPlayMode::ONCE;
    splashAnim.onComplete = [this]() { this->fadeAnim.start(); };
    splashAnim.start();

    // B
    splashAnim.addEvent(0.0f, []() { Game::PlaySound("assets/sfx/swosh-05.ogg"); });
    animB.interpolationType = InterpolationType::EASE_IN;
    animB.addKeyframe(0.0f, {-20, Game::ScreenHeight() / 2});
    animB.addKeyframe(0.1f, {textStart + 40, Game::ScreenHeight() / 2});

    animB.addKeyframe(0.70f, {textStart + 40, Game::ScreenHeight() / 2});
    animB.addKeyframe(0.72f, {textStart - 25, Game::ScreenHeight() / 2});
    animB.addKeyframe(0.90f, {textStart, Game::ScreenHeight() / 2});

    // L
    splashAnim.addEvent(0.1f, []() { Game::PlaySound("assets/sfx/swosh-18.ogg"); });
    animL.interpolationType = InterpolationType::EASE_IN;
    animL.addKeyframe(0.1f, {textStart + 20, Game::ScreenHeight() + 20});
    animL.addKeyframe(0.2f, {textStart + 40, Game::ScreenHeight() / 2});

    animL.addKeyframe(0.70f, {textStart + 40, Game::ScreenHeight() / 2});
    animL.addKeyframe(0.72f, {textStart + 20 - 20, Game::ScreenHeight() / 2});
    animL.addKeyframe(0.90f, {textStart + 20, Game::ScreenHeight() / 2});

    // T
    splashAnim.addEvent(0.2f, []() { Game::PlaySound("assets/sfx/swosh-15.ogg"); });
    animT.interpolationType = InterpolationType::EASE_IN;
    animT.addKeyframe(0.2f, {textStart + 40, -20});
    animT.addKeyframe(0.3f, {textStart + 40, Game::ScreenHeight() / 2});

    animT.addKeyframe(0.70f, {textStart + 40, Game::ScreenHeight() / 2});
    animT.addKeyframe(0.72f, {textStart + 40 - 15, Game::ScreenHeight() / 2});
    animT.addKeyframe(0.90f, {textStart + 40, Game::ScreenHeight() / 2});

    // Games
    splashAnim.addEvent(0.4f, []() { Game::PlaySound("assets/sfx/swosh-37-mod.ogg"); });
    splashAnim.addEvent(0.71f, []() { Game::PlaySound("assets/sfx/rumble.ogg"); });
    animGames.interpolationType = InterpolationType::EASE_IN;
    animGames.addKeyframe(0.5f, {Game::ScreenWidth() + 20, Game::ScreenHeight() / 2});
    animGames.addKeyframe(0.7f, {textStart + 65, Game::ScreenHeight() / 2});

    // square
    animSquare.interpolationType = InterpolationType::SNAP;
    animSquare.addKeyframe(0.0f, {0, 0});

    animSquare.addKeyframe(0.10f, {40, 40});
    animSquare.addKeyframe(0.11f, {30, 30});

    animSquare.addKeyframe(0.20f, {50, 50});
    animSquare.addKeyframe(0.21f, {40, 40});

    animSquare.addKeyframe(0.30f, {60, 60});
    animSquare.addKeyframe(0.31f, {50, 50});

    animSquare.addKeyframe(0.7f, {0, 0});

    // shake
    animShake.interpolationType = InterpolationType::LINEAR;
    animShake.addKeyframe(0.7f, 0.f);
    animShake.addKeyframe(0.71f, 10.f);
    animShake.addKeyframe(0.8f, 0.f);

    // fade
    fadeAnim.stop();
    fadeAnim.duration = 3.0f;
    fadeAnim.mode = AnimationPlayMode::ONCE;
    fadeAnim.onComplete = []() { Game::LoadScene(Scenes::MAIN_MENU); };
    animFade.interpolationType = InterpolationType::LINEAR;
    animFade.addKeyframe(1.f / 3.f, 0);
    animFade.addKeyframe(2.f / 3.f, 255);
}

void SplashScene::dispose()
{
    Log::info("Unloading splash scene\n");
    splashAnim.events.clear();
}

void SplashScene::update(float dt)
{
    if (Input::JustPressed(SDL_SCANCODE_RETURN))
    {
        Game::LoadScene(Scenes::MAIN_MENU);
        return;
    }

    splashAnim.update(dt);
    fadeAnim.update(dt);
}

void SplashScene::draw()
{
    float camShake = animShake.evaluate(splashAnim.getProgress());
    vec2 cam{(int)(camShake * randomNeg1to1()), (int)(camShake * randomNeg1to1())};

    vec2 bPos = animB.evaluate(splashAnim.getProgress());
    Game::Text(bPos.x - cam.x, bPos.y - cam.y, "B", {.valign = VerticalAlign::MIDDLE});

    vec2 lPos = animL.evaluate(splashAnim.getProgress());
    Game::Text(lPos.x - cam.x, lPos.y - cam.y, "L", {.valign = VerticalAlign::MIDDLE});

    vec2 tPos = animT.evaluate(splashAnim.getProgress());
    Game::Text(tPos.x - cam.x, tPos.y - cam.y, "T", {.valign = VerticalAlign::MIDDLE});

    vec2 squareSize = animSquare.evaluate(splashAnim.getProgress());
    SDL_Rect r = {bPos.x - cam.x, bPos.y - cam.y - squareSize.y / 2, squareSize.x, squareSize.y};
    SDL_SetRenderDrawColor(Game::GetRenderer(), 255, 255, 255, 255);
    SDL_RenderFillRect(Game::GetRenderer(), &r);

    vec2 gamesPos = animGames.evaluate(splashAnim.getProgress());
    Game::Text(gamesPos.x - cam.x, gamesPos.y - cam.y, "GAMES", {.valign = VerticalAlign::MIDDLE});

    float fadeOpacity = animFade.evaluate(fadeAnim.getProgress());
    SDL_Rect overlayRect = {
        0,
        0,
        Game::ScreenWidth(),
        Game::ScreenHeight(),
    };
    SDL_SetRenderDrawBlendMode(Game::GetRenderer(), SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(Game::GetRenderer(), 0, 0, 0, fadeOpacity);
    SDL_RenderFillRect(Game::GetRenderer(), &overlayRect);
}
