#include "scene_options.hpp"
#include "game.hpp"
#include "input_manager.hpp"

static SDL_Color DEFAULT = {180, 180, 180, 255};
static SDL_Color FOCUSED = {255, 255, 255, 255};

void OptionsScene::init()
{
    Game::SetFontSize(20);

    mPanelWidth = 400;
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

void OptionsScene::update(float dt)
{
    animPanel.update(dt);

    if (Input::JustPressed(SDL_SCANCODE_ESCAPE))
    {
        animPanel.start();
        animPanel.onComplete = []() { Game::PopScene(); };
        mState = OptionsState::EXITING;
        return;
    }

    if (Input::JustPressed(SDL_SCANCODE_DOWN))
    {
        Game::PlaySound("assets/sfx/ui_move.ogg");
        mCursor += 1;
        if (mCursor > mLines)
            mCursor = 1;
    }
    if (Input::JustPressed(SDL_SCANCODE_UP))
    {
        Game::PlaySound("assets/sfx/ui_move.ogg");
        mCursor -= 1;
        if (mCursor < 1)
            mCursor = mLines;
    }

    if (mCursor == 1)
    {
        if (Input::JustPressed(SDL_SCANCODE_RIGHT))
        {
            Game::PlaySound("assets/sfx/ui_move.ogg");
            Game::SetSoundVolume(Game::Settings().sfx_vol + 1);
        }

        if (Input::JustPressed(SDL_SCANCODE_LEFT))
        {
            Game::PlaySound("assets/sfx/ui_move.ogg");
            Game::SetSoundVolume(Game::Settings().sfx_vol - 1);
        }
    }

    if (mCursor == 2)
    {
        if (Input::JustPressed(SDL_SCANCODE_RIGHT))
            Game::SetMusicVolume(Game::Settings().music_vol + 1);

        if (Input::JustPressed(SDL_SCANCODE_LEFT))
            Game::SetMusicVolume(Game::Settings().music_vol - 1);
    }
}

void OptionsScene::drawGUI()
{
    SDL_Rect panel = {
        Game::ScreenWidth() / 2 - mPanelWidth / 2,
        Game::ScreenHeight() / 2 - mPanelHeight / 2,
        mPanelWidth,
        mPanelHeight,
    };
    if (mState == OptionsState::EXITING)
        panel.y += animPanelDropHeight.evaluate(1 - animPanel.getProgress());
    else
        panel.y += animPanelDropHeight.evaluate(animPanel.getProgress());

    SDL_SetRenderTarget(Game::GetRenderer(), mPanelTex);
    {
        SDL_SetRenderDrawBlendMode(Game::GetRenderer(), SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(Game::GetRenderer(), 0, 0, 0, 255 * 0.95f);
        SDL_RenderClear(Game::GetRenderer());

        // PANEL CONTENT BEGIN

        int sec_line = panel.h / (this->mLines + 1);

        // measurements for 1-10 bar
        int bar_width = (panel.w / 2) * 0.8f;
        int bar_height = 20;

        int bar_rect_div = bar_width / 10;
        int bar_rect_div_gap = bar_rect_div * 0.1f;
        int bar_rect_div_inner = bar_rect_div - bar_rect_div_gap * 2;

        int bar_rect_height_gap = bar_height * 0.1f;
        int bar_rect_height_inner = bar_height - bar_rect_height_gap * 2;

        SDL_Color* color;

        // SFX
        color = mCursor == 1 ? &FOCUSED : &DEFAULT;
        Game::Text(panel.w / 4, 1 * sec_line, "SFX VOL",
                   {.color = *color, .align = TextAlign::CENTER, .valign = VerticalAlign::MIDDLE});
        SDL_Rect sfx_rect = {
            3 * panel.w / 4 - bar_width / 2,
            1 * sec_line - bar_height / 2,
            bar_width,
            bar_height,
        };

        SDL_SetRenderDrawColor(Game::GetRenderer(), color->r, color->g, color->b, color->a);
        SDL_RenderFillRect(Game::GetRenderer(), &sfx_rect);

        for (int i = 1; i <= Game::Settings().sfx_vol; i++)
        {
            SDL_Rect r = {
                sfx_rect.x + bar_rect_div * (i - 1) + bar_rect_div_gap,
                sfx_rect.y + bar_rect_height_gap,
                bar_rect_div_inner,
                bar_rect_height_inner,
            };
            SDL_SetRenderDrawColor(Game::GetRenderer(), 0, 0, 0, 255);
            SDL_RenderFillRect(Game::GetRenderer(), &r);
        }

        // MUSIC
        color = mCursor == 2 ? &FOCUSED : &DEFAULT;
        Game::Text(panel.w / 4, 2 * sec_line, "MUSIC VOL",
                   {.color = *color, .align = TextAlign::CENTER, .valign = VerticalAlign::MIDDLE});
        SDL_Rect music_rect = {
            3 * panel.w / 4 - bar_width / 2,
            2 * sec_line - bar_height / 2,
            bar_width,
            bar_height,
        };
        SDL_SetRenderDrawColor(Game::GetRenderer(), color->r, color->g, color->b, color->a);
        SDL_RenderFillRect(Game::GetRenderer(), &music_rect);

        for (int i = 1; i <= Game::Settings().music_vol; i++)
        {
            SDL_Rect r = {
                music_rect.x + bar_rect_div * (i - 1) + bar_rect_div_gap,
                music_rect.y + bar_rect_height_gap,
                bar_rect_div_inner,
                bar_rect_height_inner,
            };
            SDL_SetRenderDrawColor(Game::GetRenderer(), 0, 0, 0, 255);
            SDL_RenderFillRect(Game::GetRenderer(), &r);
        }

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

void OptionsScene::dispose()
{
    SDL_DestroyTexture(mPanelTex);
}
