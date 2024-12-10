#include "block.hpp"

void Block::move(const vec2 &dir)
{
    if (dir.x == 0 && dir.y == 0)
        return;

    prevX = x;
    prevY = y;
    prevState = state;

    if (state == BlockState::UP)
    {
        if (dir.x != 0)
        {
            state = BlockState::WIDE;
            x = dir.x > 0 ? x + 1 : x - 2;
        }
        else if (dir.y != 0)
        {
            state = BlockState::LONG;
            y = dir.y > 0 ? y + 1 : y - 2;
        }
    }
    else if (state == BlockState::WIDE)
    {
        if (dir.x != 0)
        {
            state = BlockState::UP;
            x = dir.x > 0 ? x + 2 : x - 1;
        }
        else if (dir.y != 0)
        {
            y += dir.y;
        }
    }
    else if (state == BlockState::LONG)
    {
        if (dir.x != 0)
        {
            x += dir.x;
        }
        else if (dir.y != 0)
        {
            state = BlockState::UP;
            y = dir.y > 0 ? y + 2 : y - 1;
        }
    }

    // SDL_Log("block %d %d\n", x, y);
}

void Block::undoMove()
{
    x = prevX;
    y = prevY;
    state = prevState;
}

std::pair<vec2, vec2> Block::getPositions()
{
    if (state == BlockState::LONG)
    {
        return std::pair<vec2, vec2>(vec2(x, y), vec2(x, y + 1));
    }

    if (state == BlockState::WIDE)
        return std::pair<vec2, vec2>(vec2(x, y), vec2(x + 1, y));

    return std::pair<vec2,vec2>(vec2(x, y), vec2(x, y));

}

void Block::draw(int levelX, int levelY, int cellSize)
{
    SpriteID sprId = SpriteID::BLOCK_UP;
    if (state == BlockState::LONG)
        sprId = SpriteID::BLOCK_LONG;
    if (state == BlockState::WIDE)
        sprId = SpriteID::BLOCK_WIDE;
    
    int sx, sy;
    toISO(x, y, cellSize, cellSize/2, &sx, &sy);

    Game::DrawSprite(levelX + sx, levelY + sy, sprId);
}
