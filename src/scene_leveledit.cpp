#include "scene_leveledit.hpp"
#include "input_manager.hpp"
#include "log.hpp"
#include <format>
#include <math.h>
#include <string>

static const int cellSize = 64;

const char* NORMAL_MODE_INSTRUCTIONS = "[NORMAL] LMB: toggle floor    RMB: toggle start/finish    F2: save file   "
                                       " F3: save new file    F4: random    F5: reset level    NUMPAD Arrows: "
                                       "resize grid";
const char* SWITCH_MODE_INSTRUCTIONS = "[SWITCH] LMB: set switch destination    RMB: toggle floor ";

Rect levelRect;

void LevelEditScene::init()
{
    Log::info("Loading Editor scene\n");
    this->reset();
}

void LevelEditScene::dispose()
{
    Log::info("Unloading Editor scene\n");
}

void LevelEditScene::reset()
{
    auto lvl = Game::GetOrCreateState("curr_level", "0");
    lvlIdx = std::stoi(lvl);
    Log::info("Loading level %d\n", lvlIdx + 1);
    if (lvlIdx >= Game::GetLevelsSize())
        lvlIdx = 0;
    level.mModel.load(Game::GetLevelData(lvlIdx));
    level.mModel.cols = MAX_GRID_SIZE;
    level.mModel.rows = MAX_GRID_SIZE;
    level.mModel.centerCells();
    levelRect = level.mModel.rect();

    auto startPos = level.mModel.getStartPos();
    block.init(startPos, BlockState::UP);

    saved = true;
    Game::SetFontSize(16);

    resize();
}

void LevelEditScene::resize()
{
    vec2 boundBottom;
    IsoToWorld(level.mModel.cols - 1, level.mModel.rows - 1, cellSize, cellSize / 2, &boundBottom.x, &boundBottom.y);

    Game::CameraSetPosition(vec2f{(float)cellSize/2, (float)(boundBottom.y / 2)});
}

void LevelEditScene::save(bool newLevel, bool saveToFile)
{
    // save current
    level.mModel.trim();
    LevelData ld = {};
    level.mModel.toLevelData(&ld);

    if (newLevel)
    {
        lvlIdx = Game::AddLevelData(ld);
        Game::SetState("curr_level", std::to_string(lvlIdx));
    }
    else
        Game::SaveLevelData(ld, lvlIdx);

    saved = true;

    if (saveToFile)
    {
#ifndef __EMSCRIPTEN__
        char fileName[50] = {};
        sprintf(fileName, "assets-build/levels/%d.txt", lvlIdx + 1);
        FILE* f = fopen(fileName, "w");
        ld.print(f);
        fclose(f);
#endif
    }

    reset();
}

void LevelEditScene::levelChanged()
{
    saved = false;
    level.mModel.centerCells();
    levelRect = level.mModel.rect();
}

void LevelEditScene::update(float dt)
{
    Input::MousePosition(&mousePos.x, &mousePos.y);
    auto mouseWorldPos = Game::ScreenToWorld(mousePos);
    WorldToIso((int)mouseWorldPos.x, (int)mouseWorldPos.y, cellSize, cellSize / 2, &mouseIsoPos.x, &mouseIsoPos.y);

    if (switchEditing)
    {
        if (Input::JustPressed(SDL_SCANCODE_S))
        {
            switchEditing = false;
        }

        tmpSwitch.floorX = mouseIsoPos.x;
        tmpSwitch.floorY = mouseIsoPos.y;

        if (Input::MouseJustPressed(SDL_BUTTON_RIGHT))
        {
            level.mModel.toggleFloor(mouseIsoPos);
            levelChanged();
        }

        if (Input::MouseJustPressed(SDL_BUTTON_LEFT))
        {
            if (level.mModel.grid[mouseIsoPos.y][mouseIsoPos.x] == CellType::EMPTY)
            {
                level.mModel.addSwitch(tmpSwitch);
                switchEditing = false;
                levelChanged();
            }
        }

        return;
    }

    if (Input::JustPressed(SDL_SCANCODE_E))
    {
        // exit level edit
        if (level.mModel.isValid())
        {
            save(false, false);
            Game::LoadScene(Scenes::ISOLEVEL);
            return;
        }
    }

    if (level.mModel.isValidPos(mouseIsoPos))
    {
        if (Input::MouseJustPressed(SDL_BUTTON_LEFT))
        {
            level.mModel.toggleFloor(mouseIsoPos);
            levelChanged();
        }

        if (Input::MouseJustPressed(SDL_BUTTON_RIGHT))
        {
            level.mModel.toggleSpecialFloor(mouseIsoPos);
            levelChanged();
        }

        if (level.mModel.grid[mouseIsoPos.y][mouseIsoPos.x] == CellType::FLOOR)
        {
            if (Input::JustPressed(SDL_SCANCODE_S))
            {
                // enter switch edit mode if no switch in gridpos
                LevelSwitch* sw;
                if (level.mModel.hasSwitchAt(mouseIsoPos, &sw))
                {
                    level.mModel.removeSwitch(mouseIsoPos);
                    levelChanged();
                }
                else
                {
                    switchEditing = true;
                    tmpSwitch.x = mouseIsoPos.x;
                    tmpSwitch.y = mouseIsoPos.y;
                }
            }
        }
    }

    if (Input::JustPressed(SDL_SCANCODE_F2) && level.mModel.isValid())
    {
        // save current
        levelChanged();
        save(false, true);
    }
    else if (Input::JustPressed(SDL_SCANCODE_F3) && level.mModel.isValid())
    {
        // save new
        levelChanged();
        save(true, true);
    }

    if (Input::JustPressed(SDL_SCANCODE_F4))
    {
        auto randLvl = generateRandomLevel(20);
        level.mModel.load(randLvl);
        level.mModel.cols = MAX_GRID_SIZE;
        level.mModel.rows = MAX_GRID_SIZE;
        level.mModel.centerCells();
        auto startPos = level.mModel.getStartPos();
        block.init(startPos, BlockState::UP);
        return;
    }

    if (Input::JustPressed(SDL_SCANCODE_F5))
    {
        reset();
        return;
    }

    block.update(dt);
    if (block.moved)
    {
        auto positions = block.currSim.getPositions();
        level.mModel.checkAndTriggerSwitches(positions.first, positions.second);
    }
}

void LevelEditScene::draw()
{
    Game::BeginCamera();

    level.draw();
    block.draw();

    // outline
    {
        SDL_Color outlineColor = {40, 200, 80, 255}; // green
        if (!level.mModel.isValid())
            outlineColor = {200, 40, 80, 255}; // orange
        else if (!saved)
            outlineColor = {255, 255, 20, 255}; // yellow

        int topX, topY, rightX, rightY, botX, botY, leftX, leftY = 0;
        IsoToWorld(0, 0, cellSize, cellSize / 2, &topX, &topY);
        IsoToWorld(level.mModel.cols, 0, cellSize, cellSize / 2, &rightX, &rightY);
        IsoToWorld(level.mModel.cols, level.mModel.rows, cellSize, cellSize / 2, &botX, &botY);
        IsoToWorld(0, level.mModel.rows, cellSize, cellSize / 2, &leftX, &leftY);

        Game::DrawLine(topX + cellSize / 2, topY, rightX + cellSize / 2, rightY, outlineColor);
        Game::DrawLine(topX + cellSize / 2, topY, leftX + cellSize / 2, leftY, outlineColor);
        Game::DrawLine(botX + cellSize / 2, botY, rightX + cellSize / 2, rightY, outlineColor);
        Game::DrawLine(botX + cellSize / 2, botY, leftX + cellSize / 2, leftY, outlineColor);
    }

    // level switches
    for (int i = 0; i < level.mModel.switchCount; i++)
    {
        LevelSwitch& sw = level.mModel.switches[i];
        SDL_Color switchColor = {255, 180, 0, 255};
        if (sw.on)
            switchColor = {180, 255, 0, 255};
        int swX, swY, swfX, swfY;
        IsoToWorld(sw.x, sw.y, cellSize, cellSize / 2, &swX, &swY);
        IsoToWorld(sw.floorX, sw.floorY, cellSize, cellSize / 2, &swfX, &swfY);
        Game::DrawLine(swX + cellSize / 2, swY + cellSize / 4, swfX + cellSize / 2, swfY + cellSize / 4, switchColor);
    }

    // switch editing
    if (switchEditing)
    {
        int swX, swY, swfX, swfY;
        IsoToWorld(tmpSwitch.x, tmpSwitch.y, cellSize, cellSize / 2, &swX, &swY);
        IsoToWorld(tmpSwitch.floorX, tmpSwitch.floorY, cellSize, cellSize / 2, &swfX, &swfY);

        Game::DrawSprite(swX, swY, SPR_FLOOR_HIGHLIGHT);
        Game::DrawSprite(swfX, swfY, SPR_FLOOR_HIGHLIGHT);
        Game::DrawLine(swX + cellSize / 2, swY + cellSize / 4, swfX + cellSize / 2, swfY + cellSize / 4,
                       {255, 120, 0, 255});
    }

    // editor
    if (level.mModel.isValidPos(mouseIsoPos))
    {
        int mix, miy;
        IsoToWorld(mouseIsoPos.x, mouseIsoPos.y, cellSize, cellSize / 2, &mix, &miy);
        Game::DrawSprite(mix, miy, SPR_FLOOR_HIGHLIGHT);
    }

    Game::EndCamera();

    // Mode
    if (switchEditing)
        Game::Text(10, 10, SWITCH_MODE_INSTRUCTIONS);
    else
        Game::Text(10, 10, NORMAL_MODE_INSTRUCTIONS);

    // Level Info
    Game::Text(10, Game::ScreenHeight() - 35,
               std::format("Level {} {}x{} {}", lvlIdx + 1, levelRect.w, levelRect.h,
                           (level.mModel.isValid() ? "OK" : "NOK")));
}
