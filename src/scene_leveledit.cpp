#include <math.h>
#include <string>
#include <format>

#include "scene_leveledit.hpp"
#include "input_manager.hpp"

const char* NORMAL_MODE_INSTRUCTIONS = "[NORMAL] LMB: toggle floor    RMB: toggle start/finish    F2: save file    F3: save new file    NUMPAD Arrows: resize grid";
const char* SWITCH_MODE_INSTRUCTIONS = "[SWITCH] LMB: set switch destination    RMB: toggle floor ";

void LevelEditScene::init()
{
    SDL_Log("Loading Editor scene\n");
    this->reset();
}

void LevelEditScene::dispose()
{
    SDL_Log("Unloading Editor scene\n");
}

void LevelEditScene::reset()
{
    auto lvl = Game::GetOrCreateState("curr_level", "0");
    SDL_Log("Loading level %s\n", lvl.c_str());
    lvlIdx = std::stoi(lvl);
    if (lvlIdx >= Game::GetLevelsSize())
        lvlIdx = 0;
    level.load(Game::GetLevelData(lvlIdx));

    auto startPos = level.getStartPos();
    block.init(startPos, BlockState::UP);

    mEditInstruction.setPointSize(16);
    // mEditInstruction.setText("LMB: toggle floor    RMB: toggle start/finish    F2: save file    F3: save new file    NUMPAD Arrows: resize grid");
    mEditInstruction.setText(NORMAL_MODE_INSTRUCTIONS);
    mLevelText.setText(
        std::format("Level {} {}x{}", lvlIdx, level.cols, level.rows));

    resize();
}

void LevelEditScene::resize()
{
    // view sizes
    int hor = SDL_floorf(Game::ScreenWidth() * 0.8) / level.cols;
    int vert = SDL_floorf(Game::ScreenHeight() * 0.8) / level.rows;
    cellSize = hor > vert ? vert : hor;
    offsetX = Game::ScreenWidth() / 2 - level.cols * cellSize / 2;
    offsetY = Game::ScreenHeight() / 2 - level.rows * cellSize / 2;
}

void LevelEditScene::save(bool newLevel, bool saveToFile)
{
    // save current
    LevelData ld = {};
    level.toLevelData(&ld);
    if (newLevel)
        lvlIdx = Game::AddLevelData(ld);
    else
        Game::SaveLevelData(ld, lvlIdx);
    
    saved = true;
    
    if (saveToFile)
    {
        char fileName[50] = {};
        sprintf(fileName, "assets/levels/%d.txt", lvlIdx);
        FILE* f = fopen(fileName, "w");
        ld.print(f);
        fclose(f);
    }
}

void LevelEditScene::levelChanged()
{
    saved = false;
    mLevelText.setText(std::format("Level {} {}x{} {}", lvlIdx, level.cols, level.rows, (level.isValid() ? "OK" : "NOK")));
}

void toGrid(int worldX, int worldY, int cellSize, int cols, int rows, int *x, int *y)
{
    *x = SDL_floor(worldX / cellSize);
    if (worldX < 0)
        *x -= 1;
    *y = SDL_floor(worldY / cellSize);
    if (worldY < 0)
        *y -= 1;
}

void LevelEditScene::update(float dt)
{
    Input::MousePosition(&mousePos.x, &mousePos.y);
    toGrid(mousePos.x - offsetX, mousePos.y - offsetY, cellSize, level.cols, level.rows, &mouseGridPos.x, &mouseGridPos.y);

    if (switchEditing)
    {

        if (Input::JustPressed(SDL_SCANCODE_S))
        {
            switchEditing = false;
            mEditInstruction.setText(NORMAL_MODE_INSTRUCTIONS);
        }

        tmpSwitch.floorX = mouseGridPos.x;
        tmpSwitch.floorY = mouseGridPos.y;

        if (Input::MouseJustPressed(SDL_BUTTON_RIGHT))
        {
            level.toggleFloor(mouseGridPos);
            levelChanged();
        }

        if (Input::MouseJustPressed(SDL_BUTTON_LEFT)) {
            if (level.grid[mouseGridPos.y][mouseGridPos.x] == CellType::EMPTY) {
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


    if (level.isValidPos(mouseGridPos))
    {
        if (Input::MouseJustPressed(SDL_BUTTON_LEFT))
        {
            level.toggleFloor(mouseGridPos);
            levelChanged();
        }

        if (Input::MouseJustPressed(SDL_BUTTON_RIGHT))
        {
            level.toggleSpecialFloor(mouseGridPos);
            levelChanged();
        }

        if (level.grid[mouseGridPos.y][mouseGridPos.x] == CellType::FLOOR)
        {
            if (Input::JustPressed(SDL_SCANCODE_S)) {
                // enter switch edit mode if no switch in gridpos
                LevelSwitch* sw;
                if (level.hasSwitchAt(mouseGridPos, &sw)) {
                    level.removeSwitch(mouseGridPos);
                    levelChanged();
                } else {
                    switchEditing = true;
                    mEditInstruction.setText(SWITCH_MODE_INSTRUCTIONS);
                    tmpSwitch.x = mouseGridPos.x;
                    tmpSwitch.y = mouseGridPos.y;
                }
            }
        }
    }

    if (Input::JustPressed(SDL_SCANCODE_KP_6)) {
        level.addColumn();
        resize();
        levelChanged();
    }
    if (Input::JustPressed(SDL_SCANCODE_KP_8)) {
        level.removeRow();
        resize();
        levelChanged();
    }
    if (Input::JustPressed(SDL_SCANCODE_KP_4)) {
        level.removeColumn();
        resize();
        levelChanged();
    }
    if (Input::JustPressed(SDL_SCANCODE_KP_2)) {
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
        Game::SetState("curr_level", std::to_string(lvlIdx));
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

    if (moveDir.x != 0 || moveDir.y != 0)
    {
        /* allow any movement */
        block.updateMovementIntent(moveDir);
        block.x = block.moveIntent.newPos.x;
        block.y = block.moveIntent.newPos.y;
        block.state = block.moveIntent.newState;

        if (block.moveIntent.moved) {
            auto positions = block.getPositions();
            level.checkAndTriggerSwitches(positions.first, positions.second);
        }
    }
}


void LevelEditScene::draw()
{
    // outline
    SDL_Rect rr = {offsetX - 1, offsetY - 1, cellSize * level.cols + 2, cellSize * level.rows + 2};
    SDL_SetRenderDrawColor(Game::GetRenderer(), 40, 200, 80, 255); // green
    if (!level.isValid())
        SDL_SetRenderDrawColor(Game::GetRenderer(), 200, 40, 80, 255); // orange
    else if (!saved)
        SDL_SetRenderDrawColor(Game::GetRenderer(), 255, 255, 20, 255); // yellow
    SDL_RenderDrawRect(Game::GetRenderer(), &rr);

    // draw level
    for (int j = 0; j < level.rows; j++)
    {
        for (int i = 0; i < level.cols; i++)
        {
            SDL_Rect r = {
                offsetX + cellSize * i,
                offsetY + cellSize * j,
                cellSize,
                cellSize};

            switch (level.grid[j][i])
            {
            case CellType::FLOOR: // floor
                SDL_SetRenderDrawColor(Game::GetRenderer(), 100, 100, 100, 255);
                break;
            case CellType::START: // start
                SDL_SetRenderDrawColor(Game::GetRenderer(), 100, 100, 200, 255);
                break;
            case CellType::FINISH: // finish
                SDL_SetRenderDrawColor(Game::GetRenderer(), 100, 200, 100, 255);
                break;
            case CellType::GHOST: // ghost
                SDL_SetRenderDrawColor(Game::GetRenderer(), 255, 180, 0, 255);
                break;
            case CellType::EMPTY:
            default: // empty
                SDL_SetRenderDrawColor(Game::GetRenderer(), 10, 10, 10, 255);
                break;
            }
            SDL_RenderFillRect(Game::GetRenderer(), &r);
            SDL_SetRenderDrawColor(Game::GetRenderer(), 0, 0, 0, 255);
            SDL_RenderDrawRect(Game::GetRenderer(), &r);
        }
    }

    // level switches
    for (int i = 0; i < level.switchCount; i++) {
        LevelSwitch& sw = level.switches[i];
        SDL_SetRenderDrawColor(Game::GetRenderer(), 255, 180, 0, 255);
        if (sw.on)
            SDL_SetRenderDrawColor(Game::GetRenderer(), 180, 255, 0, 255);
        SDL_Rect r = {
            offsetX + cellSize * sw.x + cellSize/4,
            offsetY + cellSize * sw.y + cellSize/4,
            cellSize - cellSize/2,
            cellSize - cellSize/2};
        SDL_RenderFillRect(Game::GetRenderer(), &r);
        SDL_RenderDrawLine(Game::GetRenderer(),
            offsetX + sw.x * cellSize + cellSize/2,
            offsetY + sw.y * cellSize + cellSize/2,
            offsetX + sw.floorX * cellSize + cellSize/2,
            offsetY + sw.floorY * cellSize + cellSize/2
            );
    }

    // switch editing
    if (switchEditing)
    {
        SDL_SetRenderDrawColor(Game::GetRenderer(), 255, 120, 0, 255);
        SDL_Rect r = {
            offsetX + cellSize * tmpSwitch.x + cellSize/4,
            offsetY + cellSize * tmpSwitch.y + cellSize/4,
            cellSize - cellSize/2,
            cellSize - cellSize/2};
        SDL_RenderFillRect(Game::GetRenderer(), &r);
        SDL_RenderDrawLine(Game::GetRenderer(),
            offsetX + tmpSwitch.x * cellSize + cellSize/2,
            offsetY + tmpSwitch.y * cellSize + cellSize/2,
            offsetX + tmpSwitch.floorX * cellSize + cellSize/2,
            offsetY + tmpSwitch.floorY * cellSize + cellSize/2
            );
    }

    // draw block
    SDL_SetRenderDrawColor(Game::GetRenderer(), 200, 100, 100, 255);
    SDL_Rect blockRect = {
        offsetX + block.x * cellSize,
        offsetY + block.y * cellSize,
        cellSize,
        cellSize
    };
    if (block.state == BlockState::WIDE)
        blockRect.w *= 2;
    if (block.state == BlockState::LONG)
        blockRect.h *= 2;
    
    blockRect.x += 4;
    blockRect.y += 4;
    blockRect.w -= 8;
    blockRect.h -= 8;
    SDL_RenderFillRect(Game::GetRenderer(), &blockRect);

    // editor
    if (level.isValidPos(mouseGridPos))
    {
        SDL_SetRenderDrawColor(Game::GetRenderer(), 255, 100, 0, 255);
        SDL_Rect tileSelect = {
            .x = offsetX + mouseGridPos.x * cellSize,
            .y = offsetY + mouseGridPos.y * cellSize,
            .w = cellSize,
            .h = cellSize};
        SDL_RenderDrawRect(Game::GetRenderer(), &tileSelect);
    }

    mEditInstruction.draw(10, 10);
    mLevelText.draw(10, Game::ScreenHeight() - 35);
}

