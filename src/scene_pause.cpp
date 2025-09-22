#include "scene_pause.hpp"
#include "game.hpp"
#include "input_manager.hpp"
#include "log.hpp"
#include <format>

void PauseScene::preload()
{
    auto guiEntity = entities.add("gui");
    guiEntity->addComponent<CTransform>();
    auto gui = guiEntity->addComponent<GuiComponent>();
    GuiTheme::Set(GuiTheme::Game());

    this->panel = gui->addChild<Panel>(Game::ScreenWidth() / 2 - mPanelWidth / 2,
                                       Game::ScreenHeight() / 2 - mPanelHeight / 2, mPanelWidth, mPanelHeight);
    panel->widthSizing.sizing = AxisSizing::FIXED;
    panel->widthSizing.value = mPanelWidth;
    panel->heightSizing.sizing = AxisSizing::FIXED;
    panel->heightSizing.value = mPanelHeight;
    Log::debug("%d %d", mPanelWidth, mPanelHeight);
    panel->layout = LayoutType::TopToBottom;
    panel->justifyContent = JustifyContent::SpaceEvenly;
    panel->alignItems = AlignItems::Center;
    // panel->padding.setX(50);
    // panel->padding.setY(30);

    auto pauseTitle = panel->addChild<Label>("PAUSED");
    pauseTitle->setFontSize(32);

    auto optResume = panel->addChild<Label>("RESUME");
    auto optOptions = panel->addChild<Label>("OPTIONS");
    auto optMainMenu = panel->addChild<Label>("MAIN MENU");
    auto optExit = panel->addChild<Label>("EXIT");

    this->cursor = panel->addChild<Cursor>();
    this->cursor->enabled = true;

    cursor->addSelectable(optResume, [this]() {
        if (Input::JustPressed(SDL_SCANCODE_RETURN))
        {
            animPanel.start();
            animPanel.onComplete = []() { Game::PopScene(); };
            mState = PauseState::EXITING;
        }
    });

    cursor->addSelectable(optOptions, [this]() {
        if (Input::JustPressed(SDL_SCANCODE_RETURN))
        {
            animPanel.start();
            animPanel.onComplete = []() {
                Game::PopScene();
                Game::PushScene(Scenes::OPTIONS);
            };
            mState = PauseState::EXITING;
        }
    });

    cursor->addSelectable(optMainMenu, [this]() {
        if (Input::JustPressed(SDL_SCANCODE_RETURN))
        {
            animPanel.start();
            animPanel.onComplete = []() {
                Game::PopScene();
                Game::LoadScene(Scenes::MAIN_MENU);
            };
            mState = PauseState::EXITING;
        }
    });

    cursor->addSelectable(optExit, [this]() {
        if (Input::JustPressed(SDL_SCANCODE_RETURN))
        {
            animPanel.start();
            animPanel.onComplete = []() { Game::Exit(); };
            mState = PauseState::EXITING;
        }
    });
}

void PauseScene::init()
{
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
        mState = PauseState::EXITING;
        return;
    }

    if (Input::JustPressed(SDL_SCANCODE_UP))
    {
        Game::PlaySound("assets/sfx/ui_move.ogg");
        cursor->focusPrevious();
    }
    if (Input::JustPressed(SDL_SCANCODE_DOWN))
    {
        Game::PlaySound("assets/sfx/ui_move.ogg");
        cursor->focusNext();

    }
}

void PauseScene::drawGUI()
{
}

void PauseScene::dispose()
{
}
