#include "scene_levelselect.hpp"
#include "game.hpp"
#include "input_manager.hpp"
#include <format>

// static SDL_Color DEFAULT = {180, 180, 180, 255};
// static SDL_Color FOCUSED = {255, 255, 255, 255};

void LevelSelectScene::init()
{
    Game::SetFontSize(20);
    mTitle = std::format("Level {}", mLvlIdx + 1);
    level.init(Game::GetLevelData(mLvlIdx));

    mLevelCount = Game::GetLevelsSize();

    int w = Game::ScreenWidth() * 0.7;
    int h = Game::ScreenHeight() * 0.7;
    mPanelTex = SDL_CreateTexture(Game::GetRenderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);
    SDL_SetTextureBlendMode(mPanelTex, SDL_BLENDMODE_BLEND);

    animPanel.duration = .5f;
    animPanelDropHeight.interpolationType = InterpolationType::EASE_OUT;
    animPanelDropHeight.addKeyframe(0.f, -(h + Game::ScreenHeight() / 2));
    animPanelDropHeight.addKeyframe(1.f, 0);
    animPanel.start();
}

void LevelSelectScene::update(float dt)
{
    animPanel.update(dt);

    if (exiting)
        return;

    if (Input::JustPressed(SDL_SCANCODE_ESCAPE))
    {
        animPanel.start();
        animPanel.onComplete = []() { Game::PopScene(); };
        exiting = true;
        return;
    }

    if (Input::JustPressed(SDL_SCANCODE_RETURN))
    {
        Game::SetState("curr_level", std::to_string(mLvlIdx));
        animPanel.onComplete = []() {
            Game::PopScene();
            Game::LoadScene(Scenes::ISOLEVEL);
        };
        exiting = true;
        animPanel.start();
    }

    int loadLevel = -1;
    if (Input::JustPressed(SDL_SCANCODE_RIGHT))
    {
        mLvlIdx += 1;
        if (mLvlIdx > mLevelCount - 1)
            mLvlIdx = 0;

        Game::PlaySound("assets/sfx/ui_move.ogg");

        loadLevel = mLvlIdx;
    }
    if (Input::JustPressed(SDL_SCANCODE_LEFT))
    {
        mLvlIdx -= 1;
        if (mLvlIdx < 0)
            mLvlIdx = mLevelCount - 1;

        Game::PlaySound("assets/sfx/ui_move.ogg");

        loadLevel = mLvlIdx;
    }

    if (loadLevel >= 0)
    {
        level.init(Game::GetLevelData(loadLevel));
        loadLevel = -1;
    }
}

void LevelSelectScene::draw()
{
    int w = Game::ScreenWidth() * 0.7;
    int h = Game::ScreenHeight() * 0.7;

    SDL_Rect panel = {
        Game::ScreenWidth() / 2 - w / 2,
        Game::ScreenHeight() / 2 - h / 2,
        w,
        h,
    };
    if (exiting)
        panel.y += animPanelDropHeight.evaluate(1 - animPanel.getProgress());
    else
        panel.y += animPanelDropHeight.evaluate(animPanel.getProgress());

    SDL_SetRenderTarget(Game::GetRenderer(), mPanelTex);
    {
        SDL_SetRenderDrawBlendMode(Game::GetRenderer(), SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(Game::GetRenderer(), 0, 0, 0, 255 * 0.95f);
        SDL_RenderClear(Game::GetRenderer());

        for (int i = 0; i < mLevelCount; i++)
        {
            int lw = 40;
            int lh = 40;
            int gap = 5;
            SDL_Rect l = {20 + i * (lw + gap), 20, lw, lh};
            SDL_SetRenderDrawColor(Game::GetRenderer(), 30, 30, 30, 255);
            if (i == mLvlIdx)
                SDL_SetRenderDrawColor(Game::GetRenderer(), 60, 60, 60, 255);
            SDL_RenderFillRect(Game::GetRenderer(), &l);
            Game::Text(l.x + l.w / 2, l.y + l.h / 2, std::format("{}", i + 1),
                       {.align = TextAlign::CENTER, .valign = VerticalAlign::MIDDLE});
        }

        level.draw(w / 2, h / 2 - 100, 64);

        SDL_SetRenderTarget(Game::GetRenderer(), nullptr);
    }
    SDL_RenderCopy(Game::GetRenderer(), mPanelTex, nullptr, &panel);

    SDL_Rect panel_border = {
        panel.x - 1,
        panel.y - 1,
        panel.w + 2,
        panel.h + 2,
    };
    SDL_SetRenderDrawColor(Game::GetRenderer(), 255, 255, 255, 255);
    SDL_RenderDrawRect(Game::GetRenderer(), &panel_border);
}

void LevelSelectScene::dispose()
{
    SDL_DestroyTexture(mPanelTex);
}
