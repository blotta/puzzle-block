#include "scene_levelselect.hpp"
#include "game.hpp"
#include "input_manager.hpp"
#include <format>

const int cellSize = 64;

class LevelSelectWidget : public Widget
{
  private:
    int levelIdx = 0;
    Level level;
    vec2 centerOffset;

  public:
    LevelSelectWidget(Widget* parent, int levelIdx) : Widget(parent)
    {
        layout = LayoutType::None;
        setLevel(levelIdx);
    }

    void setLevel(int levelIdx)
    {
        this->levelIdx = levelIdx;
        level.load(Game::GetLevelData(levelIdx));
        Rect r = level.rect();
        vec2 isoCenter = r.center();
        IsoToWorld(isoCenter.x, isoCenter.y, cellSize, cellSize / 2, &centerOffset.x, &centerOffset.y);
        centerOffset = centerOffset + vec2{cellSize, cellSize / 2};
    }

    void draw() override
    {
        int x = this->parent->rect.center().x - centerOffset.x;
        int y = this->parent->rect.center().y - centerOffset.y;

        for (int i = 0; i < level.cols; i++)
        {
            for (int j = 0; j < level.rows; j++)
            {

                if (level.grid[j][i] == CellType::EMPTY)
                    continue;

                SpriteID sprId = SpriteID::SPR_FLOOR;

                switch (level.grid[j][i])
                {
                case CellType::START:
                    sprId = SpriteID::SPR_FLOOR_START;
                    break;
                case CellType::FINISH:
                    sprId = SpriteID::SPR_FLOOR_FINISH;
                    break;
                case CellType::THIN:
                    sprId = SpriteID::SPR_FLOOR_THIN;
                    break;
                default:
                    sprId = SpriteID::SPR_FLOOR;
                    break;
                }

                int sx;
                int sy;
                IsoToWorld(i, j, cellSize, cellSize / 2, &sx, &sy);

                vec2 posDiff = {0, 0};

                Game::DrawSprite(x + sx + posDiff.x, y + sy + posDiff.y, sprId);

                for (int sidx = 0; sidx < this->level.switchCount; sidx++)
                {
                    auto& sw = this->level.switches[sidx];
                    if (sw.x != i || sw.y != j)
                        continue;

                    int sx;
                    int sy;
                    IsoToWorld(sw.x, sw.y, cellSize, cellSize / 2, &sx, &sy);

                    Game::DrawSprite(x + sx + posDiff.x, y + sy + posDiff.y,
                                     sw.on ? SpriteID::SPR_SWITCH_ON : SpriteID::SPR_SWITCH_OFF);
                }
            }
        }

        // Game::DrawPoint(x, y, {255, 0, 0, 255});
    }
};

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

    for (int i = 0; i < Game::GetLevelsSize(); i++)
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

    auto levelVisualPanel = panel->addChild<Panel>(0, 0);
    levelVisualPanel->widthSizing.sizing = AxisSizing::GROW;
    levelVisualPanel->heightSizing.sizing = AxisSizing::GROW;

    this->levelSelectWidget = levelVisualPanel->addChild<LevelSelectWidget>(0);
}

void LevelSelectScene::init()
{
    animPanel.duration = .5f;
    animPanelDropHeight.interpolationType = InterpolationType::EASE_OUT;
    animPanelDropHeight.addKeyframe(0.f, -(mPanelHeight + Game::ScreenHeight() / 2));
    animPanelDropHeight.addKeyframe(1.f, 0);
    animPanel.start();
}

void LevelSelectScene::update(float dt)
{
    animPanel.update(dt);

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

        auto it = std::find(this->cursor->selectables.begin(), this->cursor->selectables.end(), this->cursor->selected);
        int levelIdx = std::distance(this->cursor->selectables.begin(), it);
        this->levelSelectWidget->setLevel(levelIdx);

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

        auto it = std::find(this->cursor->selectables.begin(), this->cursor->selectables.end(), this->cursor->selected);
        int levelIdx = std::distance(this->cursor->selectables.begin(), it);
        this->levelSelectWidget->setLevel(levelIdx);

        auto selectedW = cursor->selected->rect.right() - cursor->selectables[0]->rect.x;
        auto W = levelListCont->rect.w - 20;
        int scrollX = 0;
        if (selectedW - W > 0)
        {
            scrollX = selectedW - W;
        }

        levelListCont->scrollX = scrollX;
    }
}

void LevelSelectScene::drawGUI()
{
}

void LevelSelectScene::dispose()
{
}
