#include "scene_isolevel.hpp"

IsoLevelScene::IsoLevelScene()
{
    SDL_Log("Loading IsoLevel scene\n");

    this->reset();
}

IsoLevelScene::~IsoLevelScene()
{
}

void IsoLevelScene::reset()
{
    auto lvl = Game::GetOrCreateState("next_level", "1");
    SDL_Log("Loading level %s\n", lvl.c_str());
    auto lvlIdx = std::stoi(lvl) - 1;
    level.load(Game::GetLevelData(lvlIdx));

    mText.setText("Level " + lvl);

    // view sizes
    cellSize = 64;
    vec2 boundLeft, boundRight;
    toISO(0, level.rows - 1, cellSize, cellSize/2, &boundLeft.x, &boundLeft.y);
    toISO(level.cols - 1, 0, cellSize, cellSize/2, &boundRight.x, &boundRight.y);
    int width = boundRight.x + cellSize/2 - boundLeft.x;

    vec2 boundTop, boundBottom;
    toISO(0, 0, cellSize, cellSize/2, &boundTop.x, &boundTop.y);
    toISO(level.cols - 1, level.rows - 1, cellSize, cellSize/2, &boundBottom.x, &boundBottom.y);
    int height = boundBottom.y + cellSize/2/2 - boundTop.y;

    offsetX = ((Game::ScreenWidth() - width) / 2) - boundLeft.x - cellSize / 2;
    offsetY = (Game::ScreenHeight() - height) / 2;

    auto startPos = level.getStartPos();
    block.x = startPos.x;
    block.y = startPos.y;
    block.state = BlockState::UP;
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
        auto idx = std::stoi(Game::GetState("next_level"));
        Game::SetState("next_level", std::to_string(idx + 1));
        reset();
    }
    else if (Input::JustPressed(SDL_SCANCODE_H))
    {
        auto idx = std::stoi(Game::GetState("next_level"));
        Game::SetState("next_level", std::to_string(idx - 1));
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


    block.move(moveDir);
    auto positions = block.getPositions();
    if (!level.hasFloorAt(positions.first) || !level.hasFloorAt(positions.second))
        block.undoMove();
}

void IsoLevelScene::draw()
{
    level.draw(offsetX, offsetY, cellSize);
    block.draw(offsetX, offsetY, cellSize);

    SDL_SetRenderDrawColor(Game::GetRenderer(), 255, 0, 0, 255);
    if (Input::MousePressed(SDL_BUTTON_LEFT))
        SDL_SetRenderDrawColor(Game::GetRenderer(), 0, 255, 100, 255);
    SDL_Rect r = {mousePos.x - 10, mousePos.y - 10, 20, 20};
    SDL_RenderDrawRect(Game::GetRenderer(), &r);

    mText.draw(Game::GetRenderer(), 10, 10);
}

