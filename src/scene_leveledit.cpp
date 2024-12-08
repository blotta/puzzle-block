#include <math.h>
#include <string>

#include "scene_leveledit.hpp"

LevelEditScene::LevelEditScene()
{
    SDL_Log("Loading Level scene\n");

    this->reset();
}

LevelEditScene::~LevelEditScene()
{
}

void LevelEditScene::reset()
{
    auto lvl = Game::GetOrCreateState("next_level", "1");
    SDL_Log("Loading level %s\n", lvl.c_str());
    lvlIdx = std::stoi(lvl) - 1;
    level.load(Game::GetLevelData(lvlIdx));
    mLevelText.setText("Level " + lvl);

    auto startPos = level.getStartPos();
    block.x = startPos.x;
    block.y = startPos.y;
    block.state = BlockState::UP;

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
    if (Input::JustPressed(SDL_SCANCODE_E))
    {
        // exit level edit
        Game::LoadScene(Scenes::ISOLEVEL);
        return;
    }

    Input::MousePosition(&mousePos.x, &mousePos.y);
    toGrid(mousePos.x - offsetX, mousePos.y - offsetY, cellSize, level.cols, level.rows, &mouseGridPos.x, &mouseGridPos.y);

    if (level.isValidPos(mouseGridPos))
    {
        if (Input::MouseJustPressed(SDL_BUTTON_LEFT))
        {
            level.toggleFloor(mouseGridPos);
            saved = false;
        }

        if (Input::MouseJustPressed(SDL_BUTTON_RIGHT))
        {
            level.toggleStartFinish(mouseGridPos);
            saved = false;
        }
    }

    if (Input::JustPressed(SDL_SCANCODE_KP_6)) {
        level.addColumn();
        saved = false;
        resize();
    }
    if (Input::JustPressed(SDL_SCANCODE_KP_8)) {
        level.removeRow();
        saved = false;
        resize();
    }
    if (Input::JustPressed(SDL_SCANCODE_KP_4)) {
        level.removeColumn();
        saved = false;
        resize();
    }
    if (Input::JustPressed(SDL_SCANCODE_KP_2)) {
        level.addRow();
        saved = false;
        resize();
    }

    // save
    if (Input::JustPressed(SDL_SCANCODE_S) && level.isValid())
    {
        LevelData ld = {};
        level.toLevelData(&ld);
        Game::SaveLevelData(ld, lvlIdx);
        saved = true;
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

    block.move(moveDir);
    // auto positions = block.getPositions();
    // if (!level.hasFloorAt(positions.first) || !level.hasFloorAt(positions.second))
    //     block.undoMove();
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

    mLevelText.draw(Game::GetRenderer(), 10, 10);
}
