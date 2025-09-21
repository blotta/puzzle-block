#include "scene_levelselect.hpp"
#include "game.hpp"
#include "input_manager.hpp"
#include <format>

const int OFFSCREEN_LEVEL_OFFSET = 1200;

void LevelSelectScene::preload()
{
    GuiTheme::Set(GuiTheme::Game());
    auto guiEntity = entities.add("gui");
    guiEntity->addComponent<CTransform>();
    gui = guiEntity->addComponent<GuiComponent>();

    this->panel = gui->addChild<Panel>(Game::ScreenWidth() / 2 - mPanelWidth / 2,
                                       Game::ScreenHeight() / 2 - mPanelHeight / 2, mPanelWidth, mPanelHeight);
    panel->widthSizing.sizing = AxisSizing::FIXED;
    panel->widthSizing.value = mPanelWidth;
    panel->heightSizing.sizing = AxisSizing::FIXED;
    panel->heightSizing.value = mPanelHeight;
    panel->layout = LayoutType::Row;
    panel->justifyContent = JustifyContent::Start;
    panel->alignItems = AlignItems::Stretch;
    panel->padding.setX(50);
    panel->padding.setY(20);

    this->levelListCont = panel->addChild<Panel>(0, 0);
    levelListCont->widthSizing.sizing = AxisSizing::GROW;
    levelListCont->heightSizing.sizing = AxisSizing::FIXED;
    levelListCont->heightSizing.value = 60;
    levelListCont->layout = LayoutType::Column;
    levelListCont->scrollableX = true;

    this->cursor = gui->addChild<Cursor>();
    this->cursor->enabled = true;

    for (int i = 0; i < Game::GetLevelsSize() + 5; i++)
    {
        auto levelItem = levelListCont->addChild<Panel>(50, 50);
        levelItem->widthSizing.sizing = AxisSizing::FIXED;
        levelItem->widthSizing.value = 50;
        levelItem->heightSizing.sizing = AxisSizing::FIXED;
        levelItem->heightSizing.value = 50;
        levelItem->layout = LayoutType::Column;
        levelItem->justifyContent = JustifyContent::Center;
        levelItem->alignItems = AlignItems::Center;
        levelItem->backgroundColor = levelItem->backgroundColor + 30;
        auto levelItemLabel = levelItem->addChild<Label>(std::format("{}", i + 1));
        levelItemLabel->setFontSize(20);

        this->cursor->addSelectable(levelItem, [this, i]() {
            if (Input::JustPressed(SDL_SCANCODE_RETURN))
            {
                Game::SetState("curr_level", std::to_string(i));
                this->animPanel.onComplete = []() {
                    Game::PopScene();
                    Game::LoadScene(Scenes::LEVEL);
                };
                this->mState = LevelSelectState::EXITING;
                this->animPanel.start();
            }
        });
    }
}

void LevelSelectScene::init()
{
    Game::SetFontSize(20);
    level.init(Game::GetLevelData(mLvlIdx));

    mLevelCount = Game::GetLevelsSize();

    // int w = Game::ScreenWidth() * 0.7;
    // int h = Game::ScreenHeight() * 0.7;
    // mPanelTex = SDL_CreateTexture(Game::GetRenderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);
    // if (mPanelTex == nullptr)
    // {
    //     Log::debug("Couldn't create render target texture: %s", SDL_GetError());
    // }
    // SDL_SetTextureBlendMode(mPanelTex, SDL_BLENDMODE_BLEND);

    animPanel.duration = .5f;
    animPanelDropHeight.interpolationType = InterpolationType::EASE_OUT;
    animPanelDropHeight.addKeyframe(0.f, -(mPanelHeight + Game::ScreenHeight() / 2));
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
        mState = LevelSelectState::EXITING;
        return;
    }

    if (Input::JustPressed(SDL_SCANCODE_RIGHT))
    {
        Game::PlaySound("assets/sfx/ui_move.ogg");
        this->cursor->focusNext();
        auto selectedW = cursor->selected->rect.right() - cursor->selectables[0]->rect.x;
        auto W = levelListCont->rect.w - 20;
        int scrollX = 0;
        if (selectedW - W > 0)
        {
            scrollX = selectedW - W;
        }

        levelListCont->scrollX = scrollX;
    }
    if (Input::JustPressed(SDL_SCANCODE_LEFT))
    {
        Game::PlaySound("assets/sfx/ui_move.ogg");
        this->cursor->focusPrevious();
        auto selectedW = cursor->selected->rect.right() - cursor->selectables[0]->rect.x;
        auto W = levelListCont->rect.w - 20;
        int scrollX = 0;
        if (selectedW - W > 0)
        {
            scrollX = selectedW - W;
        }

        levelListCont->scrollX = scrollX;
    }

    // if (Input::JustPressed(SDL_SCANCODE_RIGHT))
    // {
    //     Game::PlaySound("assets/sfx/ui_move.ogg");

    //     int prevLvlIdx = mLvlIdx;

    //     mLvlIdx += 1;
    //     if (mLvlIdx > mLevelCount - 1)
    //         mLvlIdx = 0;

    //     levelAux.init(Game::GetLevelData(prevLvlIdx));
    //     level.init(Game::GetLevelData(mLvlIdx));
    //     mState = LevelSelectState::SLIDING_LEVEL_LEFT;
    //     animLevelSlide.onComplete = [this]() { this->mState = LevelSelectState::IDLE; };

    //     animLevelSlide.start();
    // }
    // if (Input::JustPressed(SDL_SCANCODE_LEFT))
    // {
    //     Game::PlaySound("assets/sfx/ui_move.ogg");

    //     int prevLvlIdx = mLvlIdx;

    //     mLvlIdx -= 1;
    //     if (mLvlIdx < 0)
    //         mLvlIdx = mLevelCount - 1;

    //     levelAux.init(Game::GetLevelData(prevLvlIdx));
    //     level.init(Game::GetLevelData(mLvlIdx));
    //     mState = LevelSelectState::SLIDING_LEVEL_RIGHT;
    //     animLevelSlide.onComplete = [this]() { this->mState = LevelSelectState::IDLE; };

    //     animLevelSlide.start();
    // }

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
    // int w = Game::ScreenWidth() * 0.7;
    // int h = Game::ScreenHeight() * 0.7;

    // SDL_Rect panel = {
    //     Game::ScreenWidth() / 2 - w / 2,
    //     Game::ScreenHeight() / 2 - h / 2,
    //     w,
    //     h,
    // };
    // if (mState == LevelSelectState::EXITING)
    //     panel.y += animPanelDropHeight.evaluate(1 - animPanel.getProgress());
    // else
    //     panel.y += animPanelDropHeight.evaluate(animPanel.getProgress());

    // SDL_SetRenderTarget(Game::GetRenderer(), mPanelTex);
    // {
    //     SDL_SetRenderDrawBlendMode(Game::GetRenderer(), SDL_BLENDMODE_BLEND);
    //     SDL_SetRenderDrawColor(Game::GetRenderer(), 0, 0, 0, 255 * 0.95f);
    //     SDL_RenderClear(Game::GetRenderer());

    //     int levelNumberBaseX = 20;
    //     int levelNumberBaseY = 20;
    //     int levelNumberWidth = 40;
    //     int levelNumberHeight = 40;
    //     int levelNumberGap = 5;

    //     int cursorX = mLvlIdx * levelNumberWidth + mLvlIdx * levelNumberGap;
    //     int diff = cursorX - panel.w * 3 / 4 > 0 ? cursorX - panel.w * 3 / 4 : 0;
    //     static int diffState = 0;
    //     diffState = easings::easeOut(diffState, diff, 0.1);

    //     for (int i = 0; i < mLevelCount; i++)
    //     {
    //         int offsetX = i * levelNumberWidth + (i)*levelNumberGap;

    //         SDL_SetRenderDrawColor(Game::GetRenderer(), 30, 30, 30, 255);
    //         if (i == mLvlIdx)
    //         {
    //             SDL_SetRenderDrawColor(Game::GetRenderer(), 60, 60, 60, 255);
    //         }

    //         SDL_Rect levelNumberRect = {
    //             levelNumberBaseX + offsetX - diffState,
    //             levelNumberBaseY,
    //             levelNumberWidth,
    //             levelNumberHeight,
    //         };
    //         SDL_RenderFillRect(Game::GetRenderer(), &levelNumberRect);
    //         Game::Text(levelNumberRect.x + levelNumberRect.w / 2, levelNumberRect.y + levelNumberRect.h / 2,
    //                    std::format("{}", i + 1), {.align = TextAlign::CENTER, .valign = VerticalAlign::MIDDLE});
    //     }

    //     level.draw();
    //     levelAux.draw();

    //     SDL_SetRenderTarget(Game::GetRenderer(), nullptr);
    // }
    // SDL_RenderCopy(Game::GetRenderer(), mPanelTex, nullptr, &panel);

    // SDL_Rect panel_border = {
    //     panel.x - 1,
    //     panel.y - 1,
    //     panel.w + 2,
    //     panel.h + 2,
    // };
    // SDL_SetRenderDrawColor(Game::GetRenderer(), 255, 255, 255, 255);
    // SDL_RenderDrawRect(Game::GetRenderer(), &panel_border);
}

void LevelSelectScene::dispose()
{
    // SDL_DestroyTexture(mPanelTex);
}
