#include "scene_isolevel.hpp"

IsoLevelScene::IsoLevelScene(Game *game)
    :
    Scene(game)
    ,
    level(game), block(game)
{
    SDL_Log("Loading IsoLevel scene\n");

    this->reset();
}

IsoLevelScene::~IsoLevelScene()
{
}

void IsoLevelScene::reset()
{
    auto lvl = game->getOrCreateState("next_level", "1");
    SDL_Log("Loading level %s\n", lvl.c_str());
    auto lvlIdx = std::stoi(lvl) - 1;
    level.load(game->getLevel(lvlIdx));

    // view sizes
    cellSize = 64; // * 2;
    vec2 boundLeft, boundRight;
    toISO(0, level.rows - 1, cellSize, cellSize/2, &boundLeft.x, &boundLeft.y);
    toISO(level.cols - 1, 0, cellSize, cellSize/2, &boundRight.x, &boundRight.y);
    int width = boundRight.x + cellSize/2 - boundLeft.x;

    vec2 boundTop, boundBottom;
    toISO(0, 0, cellSize, cellSize/2, &boundTop.x, &boundTop.y);
    toISO(level.cols - 1, level.rows - 1, cellSize, cellSize/2, &boundBottom.x, &boundBottom.y);
    int height = boundBottom.y + cellSize/2/2 - boundTop.y;

    offsetX = ((game->ScreenWidth - width) / 2) - boundLeft.x - cellSize / 2;
    offsetY = (game->ScreenHeight - height) / 2;

    auto startPos = level.getStartPos();
    block.x = startPos.x;
    block.y = startPos.y;
    block.state = BlockState::UP;
}

void IsoLevelScene::input()
{
    if (game->input.just_pressed(SDL_SCANCODE_L))
    {
        auto idx = std::stoi(game->getState("next_level"));
        game->setState("next_level", std::to_string(idx + 1));
        reset();
    }
    else if (game->input.just_pressed(SDL_SCANCODE_H))
    {
        auto idx = std::stoi(game->getState("next_level"));
        game->setState("next_level", std::to_string(idx - 1));
        reset();
    }

    moveDir = vec2(0, 0);
    if (game->input.just_pressed(SDL_SCANCODE_UP))
        moveDir = vec2(0, -1);
    if (game->input.just_pressed(SDL_SCANCODE_DOWN))
        moveDir = vec2(0, 1);
    if (game->input.just_pressed(SDL_SCANCODE_LEFT))
        moveDir = vec2(-1, 0);
    if (game->input.just_pressed(SDL_SCANCODE_RIGHT))
        moveDir = vec2(1, 0);
}

void IsoLevelScene::update(float dt)
{
    block.move(moveDir);
    auto positions = block.getPositions();
    if (!level.hasFloorAt(positions.first) || !level.hasFloorAt(positions.second))
        block.undoMove();
}

void IsoLevelScene::draw()
{
    level.drawISO(game->getRenderer(), offsetX, offsetY, cellSize);
    block.drawISO(game->getRenderer(), offsetX, offsetY, cellSize);
}

