#include "scene_levelselect.hpp"
#include "game.hpp"
#include "input_manager.hpp"
#include <format>

const int OFFSCREEN_LEVEL_OFFSET = 1200;

void LevelSelectScene::init()
{
    Game::SetFontSize(20);
    level.init(Game::GetLevelData(mLvlIdx));

    mLevelCount = Game::GetLevelsSize();

    int w = Game::ScreenWidth() * 0.7;
    int h = Game::ScreenHeight() * 0.7;
    mPanelTex = SDL_CreateTexture(Game::GetRenderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);
    if (mPanelTex == nullptr)
    {
        Log::debug("Couldn't create render target texture: %s", SDL_GetError());
    }
    SDL_SetTextureBlendMode(mPanelTex, SDL_BLENDMODE_BLEND);

    animPanel.duration = .5f;
    animPanelDropHeight.interpolationType = InterpolationType::EASE_OUT;
    animPanelDropHeight.addKeyframe(0.f, -(h + Game::ScreenHeight() / 2));
    animPanelDropHeight.addKeyframe(1.f, 0);
    animPanel.start();

    animLevelSlide.duration = .2f;
    animLevelSlideLeft.interpolationType = InterpolationType::EASE_OUT;
    animLevelSlideLeft.addKeyframe(0.f, OFFSCREEN_LEVEL_OFFSET);
    animLevelSlideLeft.addKeyframe(1.f, 0);
}

void LevelSelectScene::update(float dt)
{
    animPanel.update(dt);
    animLevelSlide.update(dt);

    if (mState == LevelSelectState::EXITING)
        return;

    if (Input::JustPressed(SDL_SCANCODE_ESCAPE))
    {
        animPanel.start();
        animPanel.onComplete = []() { Game::PopScene(); };
        mState = LevelSelectState::EXITING;
        return;
    }

    if (Input::JustPressed(SDL_SCANCODE_RETURN))
    {
        Game::SetState("curr_level", std::to_string(mLvlIdx));
        animPanel.onComplete = []() {
            Game::PopScene();
            Game::LoadScene(Scenes::ISOLEVEL);
        };
        mState = LevelSelectState::EXITING;
        animPanel.start();
    }

    if (Input::JustPressed(SDL_SCANCODE_RIGHT))
    {
        Game::PlaySound("assets/sfx/ui_move.ogg");

        int prevLvlIdx = mLvlIdx;

        mLvlIdx += 1;
        if (mLvlIdx > mLevelCount - 1)
            mLvlIdx = 0;

        levelAux.init(Game::GetLevelData(prevLvlIdx));
        level.init(Game::GetLevelData(mLvlIdx));
        mState = LevelSelectState::SLIDING_LEVEL_LEFT;
        animLevelSlide.onComplete = [this]() { this->mState = LevelSelectState::IDLE; };

        animLevelSlide.start();
    }
    if (Input::JustPressed(SDL_SCANCODE_LEFT))
    {
        Game::PlaySound("assets/sfx/ui_move.ogg");

        int prevLvlIdx = mLvlIdx;

        mLvlIdx -= 1;
        if (mLvlIdx < 0)
            mLvlIdx = mLevelCount - 1;

        levelAux.init(Game::GetLevelData(prevLvlIdx));
        level.init(Game::GetLevelData(mLvlIdx));
        mState = LevelSelectState::SLIDING_LEVEL_RIGHT;
        animLevelSlide.onComplete = [this]() { this->mState = LevelSelectState::IDLE; };

        animLevelSlide.start();
    }

    int w = Game::ScreenWidth() * 0.7;
    int h = Game::ScreenHeight() * 0.7;
    if (mState == LevelSelectState::SLIDING_LEVEL_LEFT)
    {
        int lx = animLevelSlideLeft.evaluate(animLevelSlide.getProgress());

        level.pos = vec2f{(float)(w / 2 + lx), (float)(h / 2 - 100)};
        levelAux.pos = vec2f{(float)(w / 2 - OFFSCREEN_LEVEL_OFFSET + lx), (float)(h / 2 - 100)};
    }
    else if (mState == LevelSelectState::SLIDING_LEVEL_RIGHT)
    {
        int lx = -1 * animLevelSlideLeft.evaluate(animLevelSlide.getProgress());

        level.pos = vec2f{(float)(w / 2 + lx), (float)(h / 2 - 100)};
        levelAux.pos = vec2f{(float)(w / 2 + OFFSCREEN_LEVEL_OFFSET + lx), (float)(h / 2 - 100)};
    }
    else
    {
        level.pos = vec2f{(float)(w / 2), (float)(h / 2 - 100)};
    }
}

void LevelSelectScene::drawGUI()
{
    int w = Game::ScreenWidth() * 0.7;
    int h = Game::ScreenHeight() * 0.7;

    SDL_Rect panel = {
        Game::ScreenWidth() / 2 - w / 2,
        Game::ScreenHeight() / 2 - h / 2,
        w,
        h,
    };
    if (mState == LevelSelectState::EXITING)
        panel.y += animPanelDropHeight.evaluate(1 - animPanel.getProgress());
    else
        panel.y += animPanelDropHeight.evaluate(animPanel.getProgress());

    SDL_SetRenderTarget(Game::GetRenderer(), mPanelTex);
    {
        SDL_SetRenderDrawBlendMode(Game::GetRenderer(), SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(Game::GetRenderer(), 0, 0, 0, 255 * 0.95f);
        SDL_RenderClear(Game::GetRenderer());

        int levelNumberBaseX = 20;
        int levelNumberBaseY = 20;
        int levelNumberWidth = 40;
        int levelNumberHeight = 40;
        int levelNumberGap = 5;

        int cursorX = mLvlIdx * levelNumberWidth + mLvlIdx * levelNumberGap;
        int diff = cursorX - panel.w * 3 / 4 > 0 ? cursorX - panel.w * 3 / 4 : 0;
        static int diffState = 0;
        diffState = easings::easeOut(diffState, diff, 0.1);

        for (int i = 0; i < mLevelCount; i++)
        {
            int offsetX = i * levelNumberWidth + (i)*levelNumberGap;

            SDL_SetRenderDrawColor(Game::GetRenderer(), 30, 30, 30, 255);
            if (i == mLvlIdx)
            {
                SDL_SetRenderDrawColor(Game::GetRenderer(), 60, 60, 60, 255);
            }

            SDL_Rect levelNumberRect = {
                levelNumberBaseX + offsetX - diffState,
                levelNumberBaseY,
                levelNumberWidth,
                levelNumberHeight,
            };
            SDL_RenderFillRect(Game::GetRenderer(), &levelNumberRect);
            Game::Text(levelNumberRect.x + levelNumberRect.w / 2, levelNumberRect.y + levelNumberRect.h / 2,
                       std::format("{}", i + 1), {.align = TextAlign::CENTER, .valign = VerticalAlign::MIDDLE});
        }

        level.draw();
        levelAux.draw();

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
