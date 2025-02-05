#include "input_manager.hpp"
#include "scene_isolevel.hpp"

void IsoLevelScene::init()
{
    SDL_Log("Loading IsoLevel scene\n");
    this->reset();
}

void IsoLevelScene::dispose()
{
    SDL_Log("Unloading IsoLevel scene\n");
}

void IsoLevelScene::reset()
{
    auto lvl = Game::GetOrCreateState("curr_level", "0");
    SDL_Log("Loading level %s\n", lvl.c_str());
    int lvlIdx = std::stoi(lvl);
    level.load(Game::GetLevelData(lvlIdx));

    mTitleText.setText("Level " + lvl);

    // view sizes
    cellSize = 64;
    vec2 boundLeft, boundRight;
    IsoToWorld(0, level.rows - 1, cellSize, cellSize/2, &boundLeft.x, &boundLeft.y);
    IsoToWorld(level.cols - 1, 0, cellSize, cellSize/2, &boundRight.x, &boundRight.y);
    int width = boundRight.x + cellSize/2 - boundLeft.x;

    vec2 boundTop, boundBottom;
    IsoToWorld(0, 0, cellSize, cellSize/2, &boundTop.x, &boundTop.y);
    IsoToWorld(level.cols - 1, level.rows - 1, cellSize, cellSize/2, &boundBottom.x, &boundBottom.y);
    int height = boundBottom.y + cellSize/2/2 - boundTop.y;

    offsetX = ((Game::ScreenWidth() - width) / 2) - boundLeft.x - cellSize / 2;
    offsetY = (Game::ScreenHeight() - height) / 2;

    // block setup
    auto startPos = level.getStartPos();
    block.init(startPos, BlockState::UP);

    mLevelClearedText.setText("LEVEL CLEARED!");
    mLevelClearedText.hAlign = 1;
    mLevelClearedTimer.setDuration(3);
}

void IsoLevelScene::update(float dt)
{
    if (Input::JustPressed(SDL_SCANCODE_E))
    {
        // edit this level
        Game::LoadScene(Scenes::LEVEL_EDIT);
        return;
    }

    if (Input::JustPressed(SDL_SCANCODE_L))
    {
        // next level
        int idx = cycleIndex(std::stoi(Game::GetState("curr_level")), Game::GetLevelsSize(), 1);
        Game::SetState("curr_level", std::to_string(idx));
        reset();
    }
    else if (Input::JustPressed(SDL_SCANCODE_H))
    {
        // previous level
        int idx = cycleIndex(std::stoi(Game::GetState("curr_level")), Game::GetLevelsSize(), -1);
        Game::SetState("curr_level", std::to_string(idx));
        reset();
    }

    Input::MousePosition(&mousePos.x, &mousePos.y);

    moveDir = vec2(0, 0);
    if (Input::JustPressed(SDL_SCANCODE_UP))
        moveDir = vec2(0, -1);
    if (Input::JustPressed(SDL_SCANCODE_DOWN))
        moveDir = vec2(0, 1);
    if (Input::JustPressed(SDL_SCANCODE_LEFT))
        moveDir = vec2(-1, 0);
    if (Input::JustPressed(SDL_SCANCODE_RIGHT))
        moveDir = vec2(1, 0);

    bool blockMoved = block.move(moveDir, level, true);

    if (blockMoved) {
        auto positions = block.getPositions();
        level.checkAndTriggerSwitches(positions.first, positions.second);

        if (!mLevelCleared && level.cellAt(positions.first) == CellType::FINISH && level.cellAt(positions.second) == CellType::FINISH)
        {
            mLevelCleared = true;
            mLevelClearedTimer.reset();
        }
    }
    

    if (mLevelCleared && mLevelClearedTimer.isDone())
    {
        int idx = cycleIndex(std::stoi(Game::GetState("curr_level")), Game::GetLevelsSize(), 1);
        Game::SetState("curr_level", std::to_string(idx));
        Game::LoadScene(Scenes::ISOLEVEL);
    }

    
}

void IsoLevelScene::draw()
{
    level.draw(offsetX, offsetY, cellSize);
    block.draw(offsetX, offsetY, cellSize);

    mTitleText.draw(10, 10);

    if (mLevelCleared)
        mLevelClearedText.draw(Game::ScreenWidth() / 2, Game::ScreenHeight() / 4);
}

