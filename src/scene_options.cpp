#include "scene_options.hpp"
#include "game.hpp"
#include "input_manager.hpp"

void OptionsScene::preload()
{
    auto guiEntity = entities.add("gui");
    guiEntity->addComponent<CTransform>();
    this->gui = guiEntity->addComponent<GuiComponent>();

    this->panel = gui->addChild<Panel>(Game::ScreenWidth() / 2 - mPanelWidth / 2,
                                       Game::ScreenHeight() / 2 - mPanelHeight / 2, mPanelWidth, mPanelHeight);
    panel->layout = LayoutType::Column;
    panel->justifyContent = JustifyContent::SpaceEvenly;
    panel->alignItems = AlignItems::Stretch;
    panel->padding.setX(50);

    auto sfxCont = panel->addChild<Container>();
    sfxCont->layout = LayoutType::Row;
    sfxCont->justifyContent = JustifyContent::SpaceBetween;
    sfxCont->alignItems = AlignItems::Center;
    sfxCont->addChild<Label>("SFX VOL");
    auto sfxProg = sfxCont->addChild<ProgressBar>(0, 0, (int)(.3f * mPanelWidth), 15);
    sfxProg->maxValue = 10;
    sfxProg->value = Game::Settings().sfx_vol;

    auto musicCont = panel->addChild<Container>();
    musicCont->layout = LayoutType::Row;
    musicCont->justifyContent = JustifyContent::SpaceBetween;
    musicCont->alignItems = AlignItems::Center;
    musicCont->addChild<Label>("MUSIC VOL");
    auto musicProg = musicCont->addChild<ProgressBar>(0, 0, (int)(.3f * mPanelWidth), 15);
    musicProg->maxValue = 10;
    musicProg->value = Game::Settings().music_vol;

    this->cursor = gui->addChild<Cursor>();
    cursor->addSelectable(sfxCont, [sfxProg] {
        if (Input::JustPressed(SDL_SCANCODE_RIGHT))
        {
            Game::PlaySound("assets/sfx/ui_move.ogg");
            Game::SetSoundVolume(Game::Settings().sfx_vol + 1);
            sfxProg->value = Game::Settings().sfx_vol;
        };
        if (Input::JustPressed(SDL_SCANCODE_LEFT))
        {
            Game::PlaySound("assets/sfx/ui_move.ogg");
            Game::SetSoundVolume(Game::Settings().sfx_vol - 1);
            sfxProg->value = Game::Settings().sfx_vol;
        };
    });
    cursor->addSelectable(musicCont, [musicProg] {
        if (Input::JustPressed(SDL_SCANCODE_RIGHT))
        {
            Game::PlaySound("assets/sfx/ui_move.ogg");
            Game::SetMusicVolume(Game::Settings().music_vol + 1);
            musicProg->value = Game::Settings().music_vol;
        };
        if (Input::JustPressed(SDL_SCANCODE_LEFT))
        {
            Game::PlaySound("assets/sfx/ui_move.ogg");
            Game::SetMusicVolume(Game::Settings().music_vol - 1);
            musicProg->value = Game::Settings().music_vol;
        };
    });
}

void OptionsScene::init()
{
    Game::SetFontSize(20);

    animPanel.duration = .5f;
    animPanelDropHeight.interpolationType = InterpolationType::EASE_OUT;
    animPanelDropHeight.addKeyframe(0.f, -(mPanelHeight + Game::ScreenHeight() / 2));
    animPanelDropHeight.addKeyframe(1.f, 0);
    animPanel.onComplete = [this]() { cursor->enabled = true; };
    animPanel.start();
}

void OptionsScene::update(float dt)
{
    animPanel.update(dt);

    if (mState == OptionsState::EXITING)
    {
        this->panel->rect.y = this->panel->rectInit.y + animPanelDropHeight.evaluate(1 - animPanel.getProgress());
    }
    else
    {
        this->panel->rect.y = this->panel->rectInit.y + animPanelDropHeight.evaluate(animPanel.getProgress());
    }

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
        cursor->focusNext();
    }
    if (Input::JustPressed(SDL_SCANCODE_UP))
    {
        Game::PlaySound("assets/sfx/ui_move.ogg");
        cursor->focusPrevious();
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
    {
        this->panel->rect.y = this->panel->rectInit.y + animPanelDropHeight.evaluate(1 - animPanel.getProgress());
        panel.y += animPanelDropHeight.evaluate(1 - animPanel.getProgress());
    }
    else
    {
        this->panel->rect.y = this->panel->rectInit.y + animPanelDropHeight.evaluate(animPanel.getProgress());
        panel.y += animPanelDropHeight.evaluate(animPanel.getProgress());
    }
}

void OptionsScene::dispose()
{
}
