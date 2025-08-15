#include "scene_options.hpp"
#include "game.hpp"
#include "input_manager.hpp"

void OptionsScene::init()
{
}

void OptionsScene::update(float dt)
{
    if (Input::JustPressed(SDL_SCANCODE_ESCAPE))
    {
        Game::PopScene();
        return;
    }

    if (Input::JustPressed(SDL_SCANCODE_DOWN))
    {
        mCursor += 1;
    }
    if (Input::JustPressed(SDL_SCANCODE_UP))
    {
        mCursor -= 1;
    }

    if (mCursor > mLines)
        mCursor = 1;
    if (mCursor < 1)
        mCursor = mLines;

    if (mCursor == 1)
    {
        if (Input::JustPressed(SDL_SCANCODE_RIGHT))
            Game::SetSoundVolume(Game::Settings().sfx_vol + 1);

        if (Input::JustPressed(SDL_SCANCODE_LEFT))
            Game::SetSoundVolume(Game::Settings().sfx_vol - 1);
    }

    if (mCursor == 2)
    {
        if (Input::JustPressed(SDL_SCANCODE_RIGHT))
            Game::SetMusicVolume(Game::Settings().music_vol + 1);

        if (Input::JustPressed(SDL_SCANCODE_LEFT))
            Game::SetMusicVolume(Game::Settings().music_vol - 1);
    }
}

static SDL_Color DEFAULT = {180, 180, 180, 255};
static SDL_Color FOCUSED = {255, 255, 255, 255};

void OptionsScene::draw()
{
    int w = Game::ScreenWidth() * 0.5;
    int h = Game::ScreenHeight() * 0.5;
    SDL_Rect panel = {
        Game::ScreenWidth() / 2 - w / 2,
        Game::ScreenHeight() / 2 - h / 2,
        w,
        h,
    };

    SDL_SetRenderDrawBlendMode(Game::GetRenderer(), SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(Game::GetRenderer(), 0, 0, 0, 255 * 0.9f);
    SDL_RenderFillRect(Game::GetRenderer(), &panel);

    SDL_SetRenderDrawBlendMode(Game::GetRenderer(), SDL_BLENDMODE_NONE);
    SDL_SetRenderDrawColor(Game::GetRenderer(), 255, 255, 255, 255);
    SDL_RenderDrawRect(Game::GetRenderer(), &panel);

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
    Game::Text(panel.x + panel.w / 4, panel.y + 1 * sec_line, "SFX VOL",
               {.color = *color, .align = TextAlign::CENTER, .valign = VerticalAlign::MIDDLE});
    SDL_Rect sfx_rect = {
        panel.x + panel.w - bar_width - 20,
        panel.y + 1 * sec_line - bar_height / 2,
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
    Game::Text(panel.x + panel.w / 4, panel.y + 2 * sec_line, "MUSIC VOL",
               {.color = *color, .align = TextAlign::CENTER, .valign = VerticalAlign::MIDDLE});
    SDL_Rect music_rect = {
        panel.x + panel.w - bar_width - 20,
        panel.y + 2 * sec_line - bar_height / 2,
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
}

void OptionsScene::dispose()
{
}
