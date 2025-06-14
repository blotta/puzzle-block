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

    // block setup
    auto startPos = level.getStartPos();
    block.init(startPos, BlockState::UP);

    camera.offset = vec2( Game::ScreenWidth()/2, Game::ScreenHeight()/2 );
    int tx, ty;
    IsoToWorld(block.x, block.y, cellSize, cellSize/2, &tx, &ty);
    camera.target.x = tx;
    camera.target.y = ty;

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

    if (Input::MousePressed(SDL_BUTTON_LEFT))
    {
        camera.target.x = mousePos.x - camera.offset.x;
        camera.target.y = mousePos.y - camera.offset.y;
    }
    else
    {
        int tx, ty;
        IsoToWorld(block.moveIntent.newPos.x, block.moveIntent.newPos.y, cellSize, cellSize/2, &tx, &ty);
        camera.target.x = (float)tx;
        camera.target.y = (float)ty;
        camera.target.x += cellSize/2;
    }
    camera.update(dt);

    if (mLevelCleared && mLevelClearedTimer.isDone())
    {
        int idx = cycleIndex(std::stoi(Game::GetState("curr_level")), Game::GetLevelsSize(), 1);
        Game::SetState("curr_level", std::to_string(idx));
        Game::LoadScene(Scenes::ISOLEVEL);
    }
}

void IsoLevelScene::draw()
{
    // level.draw(offsetX, offsetY, cellSize);
    // block.draw(offsetX, offsetY, cellSize);
    level.draw(camera.offset.x - camera.pos.x, camera.offset.y - camera.pos.y, cellSize);
    block.draw(camera.offset.x - camera.pos.x, camera.offset.y - camera.pos.y, cellSize);

    mTitleText.draw(10, 10);

    if (mLevelCleared)
        mLevelClearedText.draw(Game::ScreenWidth() / 2, Game::ScreenHeight() / 4);
    
    // SDL_SetRenderDrawColor(Game::GetRenderer(), 255, 255, 255, 128);
    // SDL_RenderDrawLine(Game::GetRenderer(), Game::ScreenWidth()/2, 0, Game::ScreenWidth()/2, Game::ScreenHeight());
    // SDL_RenderDrawLine(Game::GetRenderer(), 0, Game::ScreenHeight()/2, Game::ScreenWidth(), Game::ScreenHeight()/2);
}

