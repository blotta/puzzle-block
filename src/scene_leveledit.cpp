#include "scene_leveledit.hpp"
#include "component_gui.hpp"
#include "component_leveledit.hpp"
#include "input_manager.hpp"
#include <format>

static const int cellSize = 64;

void LevelEditScene::preload()
{
    auto lvlIdx = std::stoi(Game::GetOrCreateState("curr_level", "0"));

    auto levelEntity = entities.add("level");
    levelEntity->addComponent<CTransform>();
    this->lc = levelEntity->addComponent<LevelEditComponent>(lvlIdx, Game::GetLevelData(lvlIdx));

    auto blockEntity = entities.add("block");
    blockEntity->addComponent<CTransform>();
    blockEntity->addComponent<BlockComponent>();

    auto guiEntity = entities.add("gui");
    guiEntity->addComponent<CTransform>();
    this->gui = guiEntity->addComponent<GuiComponent>();
    GuiTheme::Set(GuiTheme::Dark());

    auto sideBar = gui->addChild<Panel>(0, 0, 200, Game::ScreenHeight());
    sideBar->widthSizing.sizing = AxisSizing::FIXED;
    sideBar->widthSizing.value = 200;
    sideBar->heightSizing.sizing = AxisSizing::GROW;
    sideBar->layout = LayoutType::TopToBottom;
    sideBar->justifyContent = JustifyContent::Start;
    sideBar->alignItems = AlignItems::Stretch;

    auto sideBarTitle = sideBar->addChild<Label>("");
    lc->onChange = [sideBarTitle, this]() {
        auto st = std::format("Level {}: {}x{}", lc->lvlIdx + 1, lc->cols(), lc->rows());
        sideBarTitle->setText(st);
    };
    lc->onChange();

    auto sideBarSaveButtons = sideBar->addChild<Widget>();
    sideBarSaveButtons->widthSizing.sizing = AxisSizing::GROW;
    sideBarSaveButtons->layout = LayoutType::LeftToRight;
    sideBarSaveButtons->justifyContent = JustifyContent::SpaceBetween;
    sideBarSaveButtons->padding.set(0);

    auto btnSaveCurrent = sideBarSaveButtons->addChild<Button>("SAVE");
    btnSaveCurrent->onClickEvent = [this]() {
        Log::debug("Saving current level");
        this->lc->save(false, true);
    };

    auto btnSaveNew = sideBarSaveButtons->addChild<Button>("SAVE NEW");
    btnSaveNew->onClickEvent = [this]() {
        Log::debug("Saving new level");
        this->lc->save(true, true);
    };

    auto btnReset = sideBarSaveButtons->addChild<Button>("RESET");
    btnReset->onClickEvent = [this]() {
        Log::debug("Resetting level");
        this->lc->load(this->lc->lvlIdx, Game::GetLevelData(this->lc->lvlIdx));

        auto bc = this->entities.get("block")[0]->getComponent<BlockComponent>();
        auto startPos = lc->getStartPos();
        bc->currSim.x = startPos.x;
        bc->currSim.y = startPos.y;
        bc->currSim.state = BlockState::UP;
        bc->nextSim = bc->currSim;
    };

    auto modePanel = gui->addChild<Panel>(Game::ScreenWidth() / 2 - 70, Game::ScreenHeight() - 30, 140, 30);
    modePanel->layout = LayoutType::TopToBottom;
    modePanel->justifyContent = JustifyContent::Center;
    modePanel->alignItems = AlignItems::Center;
    modeIndicatorLabel = modePanel->addChild<Label>(0, Game::ScreenHeight() / 2, "NORMAL");
}

void LevelEditScene::init()
{
    auto bc = entities.get("block")[0]->getComponent<BlockComponent>();
    auto startPos = lc->getStartPos();
    bc->currSim.x = startPos.x;
    bc->currSim.y = startPos.y;
    bc->currSim.state = BlockState::UP;
    bc->nextSim = bc->currSim;

    int cx, cy;
    IsoToWorld(MAX_GRID_SIZE / 2, MAX_GRID_SIZE / 2, cellSize, cellSize / 2, &cx, &cy);
    camTarget = vec2f(cx + cellSize / 2, cy);
    Game::CameraSetPosition(camTarget);
}

void LevelEditScene::update(float dt)
{
    int mx, my;
    Input::MousePosition(&mx, &my);
    auto mouseWorldPos = Game::ScreenToWorld(vec2{mx, my});
    WorldToIso((int)mouseWorldPos.x, (int)mouseWorldPos.y, cellSize, cellSize / 2, &mouseIsoPos.x, &mouseIsoPos.y);

    float camSpeed = 500.f;
    if (Input::Pressed(SDL_SCANCODE_W))
    {
        camTarget.y -= camSpeed * dt;
    }
    else if (Input::Pressed(SDL_SCANCODE_S))
    {
        camTarget.y += camSpeed * dt;
    }

    if (Input::Pressed(SDL_SCANCODE_A))
    {
        camTarget.x -= camSpeed * dt;
    }
    else if (Input::Pressed(SDL_SCANCODE_D))
    {
        camTarget.x += camSpeed * dt;
    }
    Game::CameraSetTarget(camTarget);

    if (gui->isInteracting())
    {
    }
    else if (mMode == LevelEditMode::NORMAL)
    {
        if (Input::JustPressed(SDL_SCANCODE_SPACE))
        {
            Log::debug("");
        }
        if (Input::JustPressed(SDL_SCANCODE_E))
        {
            // exit level edit
            if (this->lc->isValid())
            {
                lc->save(false, false);
                Game::LoadScene(Scenes::LEVEL);
                return;
            }
        }

        if (lc->isValidPos(mouseIsoPos))
        {
            if (Input::MouseJustPressed(SDL_BUTTON_LEFT))
            {
                lc->toggleFloor(mouseIsoPos);
            }

            if (Input::MouseJustPressed(SDL_BUTTON_RIGHT))
            {
                lc->toggleSpecialFloor(mouseIsoPos);
            }

            if (lc->cellAt(mouseIsoPos) == CellType::FLOOR)
            {
                if (Input::JustPressed(SDL_SCANCODE_T))
                {
                    // enter switch edit mode if no switch in gridpos
                    LevelSwitch* sw;
                    if (lc->hasSwitchAt(mouseIsoPos, &sw))
                    {
                        lc->removeSwitch(mouseIsoPos);
                    }
                    else
                    {
                        this->mMode = LevelEditMode::SWITCH;
                        modeIndicatorLabel->setText("SWITCH");
                        this->tmpSwitch.x = this->mouseIsoPos.x;
                        this->tmpSwitch.y = this->mouseIsoPos.y;
                    }
                }
            }
        }
    }
    else if (mMode == LevelEditMode::SWITCH)
    {
        this->tmpSwitch.floorX = this->mouseIsoPos.x;
        this->tmpSwitch.floorY = this->mouseIsoPos.y;

        if (Input::JustPressed(SDL_SCANCODE_ESCAPE) || Input::JustPressed(SDL_SCANCODE_T))
        {
            mMode = LevelEditMode::NORMAL;
            modeIndicatorLabel->setText("NORMAL");
        }

        if (Input::MouseJustPressed(SDL_BUTTON_RIGHT))
        {
            lc->toggleFloor(mouseIsoPos);
        }

        if (Input::MouseJustPressed(SDL_BUTTON_LEFT))
        {
            if (lc->cellAt(mouseIsoPos) == CellType::EMPTY)
            {
                lc->addSwitch(tmpSwitch);
                mMode = LevelEditMode::NORMAL;
                modeIndicatorLabel->setText("NORMAL");
            }
        }
    }
}

void LevelEditScene::draw()
{
    if (gui->isInteracting())
    {
    }
    else if (mMode == LevelEditMode::NORMAL)
    {
        if (lc->isValidPos(mouseIsoPos))
        {
            int mix, miy;
            IsoToWorld(mouseIsoPos.x, mouseIsoPos.y, cellSize, cellSize / 2, &mix, &miy);
            Game::DrawSprite(mix, miy, SPR_FLOOR_HIGHLIGHT);
        }
    }
    else if (mMode == LevelEditMode::SWITCH)
    {
        int swX, swY, swfX, swfY;
        IsoToWorld(tmpSwitch.x, tmpSwitch.y, cellSize, cellSize / 2, &swX, &swY);
        IsoToWorld(tmpSwitch.floorX, tmpSwitch.floorY, cellSize, cellSize / 2, &swfX, &swfY);

        Game::DrawSprite(swX, swY, SPR_FLOOR_HIGHLIGHT);
        Game::DrawSprite(swfX, swfY, SPR_FLOOR_HIGHLIGHT);
        Game::DrawLine(swX + cellSize / 2, swY + cellSize / 4, swfX + cellSize / 2, swfY + cellSize / 4,
                       {255, 120, 0, 255});
    }

    // outline
    SDL_Color outlineColor = {40, 200, 80, 255}; // green
    if (!lc->isValid())
        outlineColor = {200, 40, 80, 255}; // orange
    else if (lc->isModified())
        outlineColor = {255, 255, 20, 255}; // yellow

    int topX, topY, rightX, rightY, botX, botY, leftX, leftY = 0;
    IsoToWorld(0, 0, cellSize, cellSize / 2, &topX, &topY);
    IsoToWorld(MAX_GRID_SIZE, 0, cellSize, cellSize / 2, &rightX, &rightY);
    IsoToWorld(MAX_GRID_SIZE, MAX_GRID_SIZE, cellSize, cellSize / 2, &botX, &botY);
    IsoToWorld(0, MAX_GRID_SIZE, cellSize, cellSize / 2, &leftX, &leftY);

    Game::DrawLine(topX + cellSize / 2, topY, rightX + cellSize / 2, rightY, outlineColor);
    Game::DrawLine(topX + cellSize / 2, topY, leftX + cellSize / 2, leftY, outlineColor);
    Game::DrawLine(botX + cellSize / 2, botY, rightX + cellSize / 2, rightY, outlineColor);
    Game::DrawLine(botX + cellSize / 2, botY, leftX + cellSize / 2, leftY, outlineColor);
}

void LevelEditScene::drawGUI()
{
}
