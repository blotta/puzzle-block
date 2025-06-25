#include "scene_leveledit.hpp"
#include "input_manager.hpp"
#include "log.hpp"
#include <format>
#include <math.h>
#include <string>

const char* NORMAL_MODE_INSTRUCTIONS = "[NORMAL] LMB: toggle floor    RMB: toggle start/finish    F2: save file   "
                                       " F3: save new file    F4: random    F5: reset level    NUMPAD Arrows: "
                                       "resize grid";
const char* SWITCH_MODE_INSTRUCTIONS = "[SWITCH] LMB: set switch destination    RMB: toggle floor ";

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
    level.load(Game::GetLevelData(lvlIdx));

    auto startPos = level.getStartPos();
    block.init(startPos, BlockState::UP);

    saved = true;
    Game::SetFontSize(16);

    resize();
}

void LevelEditScene::resize()
{
    // view sizes
    cellSize = 64;
    vec2 boundLeft, boundRight;
    IsoToWorld(0, level.rows - 1, cellSize, cellSize / 2, &boundLeft.x, &boundLeft.y);
    IsoToWorld(level.cols - 1, 0, cellSize, cellSize / 2, &boundRight.x, &boundRight.y);
    int width = boundRight.x + cellSize / 2 - boundLeft.x;

    vec2 boundTop, boundBottom;
    IsoToWorld(0, 0, cellSize, cellSize / 2, &boundTop.x, &boundTop.y);
    IsoToWorld(level.cols - 1, level.rows - 1, cellSize, cellSize / 2, &boundBottom.x, &boundBottom.y);
    int height = boundBottom.y + cellSize / 2 / 2 - boundTop.y;

    offsetX = ((Game::ScreenWidth() - width) / 2) - boundLeft.x - cellSize / 2;
    offsetY = (Game::ScreenHeight() - height) / 2;
}

void LevelEditScene::save(bool newLevel, bool saveToFile)
{
    // save current
    LevelData ld = {};
    level.toLevelData(&ld);
    trimLevel(ld);

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
        char fileName[50] = {};
        sprintf(fileName, "assets/levels/%d.txt", lvlIdx + 1);
        FILE* f = fopen(fileName, "w");
        ld.print(f);
        fclose(f);
    }

    reset();
}

void LevelEditScene::levelChanged()
{
    saved = false;
}

void LevelEditScene::update(float dt)
{
    Input::MousePosition(&mousePos.x, &mousePos.y);
    WorldToIso(mousePos.x - offsetX, mousePos.y - offsetY, cellSize, cellSize / 2, &mouseIsoPos.x, &mouseIsoPos.y);
    // Log::debug("mouseIso : %d %d\n", mouseIsoPos.x, mouseIsoPos.y);

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
            level.toggleFloor(mouseIsoPos);
            levelChanged();
        }

        if (Input::MouseJustPressed(SDL_BUTTON_LEFT))
        {
            if (level.grid[mouseIsoPos.y][mouseIsoPos.x] == CellType::EMPTY)
            {
                level.addSwitch(tmpSwitch);
                switchEditing = false;
                levelChanged();
            }
        }

        return;
    }

    if (Input::JustPressed(SDL_SCANCODE_E))
    {
        // exit level edit
        if (level.isValid())
        {
            save(false, false);
            Game::LoadScene(Scenes::ISOLEVEL);
            return;
        }
    }

    if (level.isValidPos(mouseIsoPos))
    {
        if (Input::MouseJustPressed(SDL_BUTTON_LEFT))
        {
            level.toggleFloor(mouseIsoPos);
            levelChanged();
        }

        if (Input::MouseJustPressed(SDL_BUTTON_RIGHT))
        {
            level.toggleSpecialFloor(mouseIsoPos);
            levelChanged();
        }

        if (level.grid[mouseIsoPos.y][mouseIsoPos.x] == CellType::FLOOR)
        {
            if (Input::JustPressed(SDL_SCANCODE_S))
            {
                // enter switch edit mode if no switch in gridpos
                LevelSwitch* sw;
                if (level.hasSwitchAt(mouseIsoPos, &sw))
                {
                    level.removeSwitch(mouseIsoPos);
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

    if (Input::JustPressed(SDL_SCANCODE_KP_6))
    {
        level.addColumn();
        resize();
        levelChanged();
    }
    if (Input::JustPressed(SDL_SCANCODE_KP_8))
    {
        level.removeRow();
        resize();
        levelChanged();
    }
    if (Input::JustPressed(SDL_SCANCODE_KP_4))
    {
        level.removeColumn();
        resize();
        levelChanged();
    }
    if (Input::JustPressed(SDL_SCANCODE_KP_2))
    {
        level.addRow();
        resize();
        levelChanged();
    }

    if (Input::JustPressed(SDL_SCANCODE_F2) && level.isValid())
    {
        // save current
        levelChanged();
        save(false, true);
    }
    else if (Input::JustPressed(SDL_SCANCODE_F3) && level.isValid())
    {
        // save new
        levelChanged();
        save(true, true);
    }

    if (Input::JustPressed(SDL_SCANCODE_F4))
    {
        auto randLvl = generateRandomLevel(20);
        level.load(randLvl);
        auto startPos = level.getStartPos();
        block.init(startPos, BlockState::UP);
    }

    if (Input::JustPressed(SDL_SCANCODE_F5))
    {
        reset();
        return;
    }

    // normal op
    moveDir = vec2(0, 0);
    if (Input::JustPressed(SDL_SCANCODE_UP))
        moveDir = vec2(0, -1);
    if (Input::JustPressed(SDL_SCANCODE_DOWN))
        moveDir = vec2(0, 1);
    if (Input::JustPressed(SDL_SCANCODE_LEFT))
        moveDir = vec2(-1, 0);
    if (Input::JustPressed(SDL_SCANCODE_RIGHT))
        moveDir = vec2(1, 0);

    bool blockMoved = block.move(moveDir, level, false);
    if (blockMoved)
    {
        auto positions = block.getPositions();
        level.checkAndTriggerSwitches(positions.first, positions.second);
    }
}

void LevelEditScene::draw()
{
    level.draw(offsetX, offsetY, cellSize);
    block.draw(offsetX, offsetY, cellSize);

    // outline
    {
        SDL_SetRenderDrawColor(Game::GetRenderer(), 40, 200, 80, 255); // green
        if (!level.isValid())
            SDL_SetRenderDrawColor(Game::GetRenderer(), 200, 40, 80, 255); // orange
        else if (!saved)
            SDL_SetRenderDrawColor(Game::GetRenderer(), 255, 255, 20, 255); // yellow

        int topX, topY, rightX, rightY, botX, botY, leftX, leftY = 0;
        IsoToWorld(0, 0, cellSize, cellSize / 2, &topX, &topY);
        IsoToWorld(level.cols, 0, cellSize, cellSize / 2, &rightX, &rightY);
        IsoToWorld(level.cols, level.rows, cellSize, cellSize / 2, &botX, &botY);
        IsoToWorld(0, level.rows, cellSize, cellSize / 2, &leftX, &leftY);
        SDL_RenderDrawLine(Game::GetRenderer(), offsetX + topX + cellSize / 2, offsetY + topY,
                           offsetX + rightX + cellSize / 2, offsetY + rightY);
        SDL_RenderDrawLine(Game::GetRenderer(), offsetX + topX + cellSize / 2, offsetY + topY,
                           offsetX + leftX + cellSize / 2, offsetY + leftY);
        SDL_RenderDrawLine(Game::GetRenderer(), offsetX + botX + cellSize / 2, offsetY + botY,
                           offsetX + rightX + cellSize / 2, offsetY + rightY);
        SDL_RenderDrawLine(Game::GetRenderer(), offsetX + botX + cellSize / 2, offsetY + botY,
                           offsetX + leftX + cellSize / 2, offsetY + leftY);
    }

    // level switches
    for (int i = 0; i < level.switchCount; i++)
    {
        LevelSwitch& sw = level.switches[i];
        SDL_SetRenderDrawColor(Game::GetRenderer(), 255, 180, 0, 255);
        if (sw.on)
            SDL_SetRenderDrawColor(Game::GetRenderer(), 180, 255, 0, 255);
        int swX, swY, swfX, swfY;
        IsoToWorld(sw.x, sw.y, cellSize, cellSize / 2, &swX, &swY);
        IsoToWorld(sw.floorX, sw.floorY, cellSize, cellSize / 2, &swfX, &swfY);
        SDL_RenderDrawLine(Game::GetRenderer(), offsetX + swX + cellSize / 2, offsetY + swY + cellSize / 4,
                           offsetX + swfX + cellSize / 2, offsetY + swfY + cellSize / 4);
    }

    // switch editing
    if (switchEditing)
    {
        int swX, swY, swfX, swfY;
        IsoToWorld(tmpSwitch.x, tmpSwitch.y, cellSize, cellSize / 2, &swX, &swY);
        IsoToWorld(tmpSwitch.floorX, tmpSwitch.floorY, cellSize, cellSize / 2, &swfX, &swfY);
        SDL_SetRenderDrawColor(Game::GetRenderer(), 255, 120, 0, 255);
        Game::DrawSprite(swX + offsetX, swY + offsetY, SPR_FLOOR_HIGHLIGHT);
        Game::DrawSprite(swfX + offsetX, swfY + offsetY, SPR_FLOOR_HIGHLIGHT);
        SDL_RenderDrawLine(Game::GetRenderer(), offsetX + swX + cellSize / 2, offsetY + swY + cellSize / 4,
                           offsetX + swfX + cellSize / 2, offsetY + swfY + cellSize / 4);
    }

    // editor
    if (level.isValidPos(mouseIsoPos))
    {
        int mix, miy;
        IsoToWorld(mouseIsoPos.x, mouseIsoPos.y, cellSize, cellSize / 2, &mix, &miy);
        Game::DrawSprite(mix + offsetX, miy + offsetY, SPR_FLOOR_HIGHLIGHT);
    }

    // Mode
    if (switchEditing)
        Game::Text(10, 10, SWITCH_MODE_INSTRUCTIONS);
    else
        Game::Text(10, 10, NORMAL_MODE_INSTRUCTIONS);

    // Level Info
    Game::Text(10, Game::ScreenHeight() - 35,
               std::format("Level {} {}x{} {}", lvlIdx + 1, level.cols, level.rows, (level.isValid() ? "OK" : "NOK")));
}
