#include "scene_pause.hpp"
#include "game.hpp"
#include "input_manager.hpp"
#include <format>

static SDL_Color DEFAULT = {180, 180, 180, 255};
static SDL_Color FOCUSED = {255, 255, 255, 255};

void PauseScene::init()
{
    Game::SetFontSize(20);

    mPanelWidth = 250;
    mPanelHeight = 300;

    mPanelTex = SDL_CreateTexture(Game::GetRenderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, mPanelWidth,
                                  mPanelHeight);
    SDL_SetTextureBlendMode(mPanelTex, SDL_BLENDMODE_BLEND);

    animPanel.duration = .5f;
    animPanelDropHeight.interpolationType = InterpolationType::EASE_OUT;
    animPanelDropHeight.addKeyframe(0.f, -(mPanelHeight + Game::ScreenHeight() / 2));
    animPanelDropHeight.addKeyframe(1.f, 0);
    animPanel.start();
}

void PauseScene::update(float dt)
{
    animPanel.update(dt);

    if (mState == PauseState::EXITING)
        return;

    if (Input::JustPressed(SDL_SCANCODE_ESCAPE))
    {
        animPanel.start();
        animPanel.onComplete = []() { Game::PopScene(); };
        mState = PauseState::EXITING;
        return;
    }

    if (Input::JustPressed(SDL_SCANCODE_RETURN))
    {
        animPanel.onComplete = [this]() {
            Game::PopScene();

            // RESUME
            if (this->mCursor == 1)
                return;
            
            // OPTIONS
            if (this->mCursor == 2)
            {
                Game::PushScene(Scenes::OPTIONS);
                return;
            }

            // MAIN MENU
            if (this->mCursor == 3)
            {
                Game::LoadScene(Scenes::MAIN_MENU);
                return;
            }

            // EXIT
            if (this->mCursor == 4)
            {
                Game::Exit();
            }
        };
        mState = PauseState::EXITING;
        animPanel.start();
    }

    if (Input::JustPressed(SDL_SCANCODE_UP))
    {
        Game::PlaySound("assets/sfx/ui_move.ogg");
        mCursor -= 1;
        if (mCursor < 1)
            mCursor = mLines;
    }
    if (Input::JustPressed(SDL_SCANCODE_DOWN))
    {
        Game::PlaySound("assets/sfx/ui_move.ogg");

        mCursor += 1;
        if (mCursor > mLines)
            mCursor = 1;
    }
}

void PauseScene::draw()
{
    SDL_Rect panel = {
        Game::ScreenWidth() / 2 - mPanelWidth / 2,
        Game::ScreenHeight() / 2 - mPanelHeight / 2,
        mPanelWidth,
        mPanelHeight,
    };
    if (mState == PauseState::EXITING)
        panel.y += animPanelDropHeight.evaluate(1 - animPanel.getProgress());
    else
        panel.y += animPanelDropHeight.evaluate(animPanel.getProgress());

    SDL_SetRenderTarget(Game::GetRenderer(), mPanelTex);
    {
        SDL_SetRenderDrawBlendMode(Game::GetRenderer(), SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(Game::GetRenderer(), 0, 0, 0, 255 * 0.95f);
        SDL_RenderClear(Game::GetRenderer());

        // PANEL CONTENT BEGIN

        int sec_line = (mPanelHeight / (mLines + 1));

        SDL_Color* color;

        // RESUME
        color = mCursor == 1 ? &FOCUSED : &DEFAULT;
        Game::Text(mPanelWidth / 2, 1 * sec_line, "RESUME",
                   {.color = *color, .align = TextAlign::CENTER, .valign = VerticalAlign::MIDDLE});

        // OPTIONS
        color = mCursor == 2 ? &FOCUSED : &DEFAULT;
        Game::Text(mPanelWidth / 2, 2 * sec_line, "OPTIONS",
                   {.color = *color, .align = TextAlign::CENTER, .valign = VerticalAlign::MIDDLE});

        // MAIN MENU
        color = mCursor == 3 ? &FOCUSED : &DEFAULT;
        Game::Text(mPanelWidth / 2, 3 * sec_line, "MAIN MENU",
                   {.color = *color, .align = TextAlign::CENTER, .valign = VerticalAlign::MIDDLE});

        // EXIT
        color = mCursor == 4 ? &FOCUSED : &DEFAULT;
        Game::Text(mPanelWidth / 2, 4 * sec_line, "EXIT",
                   {.color = *color, .align = TextAlign::CENTER, .valign = VerticalAlign::MIDDLE});

        // PANEL CONTENT END

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

void PauseScene::dispose()
{
    SDL_DestroyTexture(mPanelTex);
}
