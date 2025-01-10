#include "block.hpp"

bool Block::move(const vec2 &dir, const Level& level, bool collide)
{
    vec2 prevPos(x,y);
    BlockState prevState = this->state;

    vec2 newPos(x,y);
    BlockState newState = this->state;

    if (dir.x == 0 && dir.y == 0)
        return false;
    
    if (state == BlockState::UP)
    {
        if (dir.x != 0)
        {
            newState = BlockState::WIDE;
            newPos.x = dir.x > 0 ? x + 1 : x - 2;
        }
        else if (dir.y != 0)
        {
            newState = BlockState::LONG;
            newPos.y = dir.y > 0 ? y + 1 : y - 2;
        }
    }
    else if (state == BlockState::WIDE)
    {
        if (dir.x != 0)
        {
            newState = BlockState::UP;
            newPos.x = dir.x > 0 ? x + 2 : x - 1;
        }
        else if (dir.y != 0)
        {
            newPos.y += dir.y;
        }
    }
    else if (state == BlockState::LONG)
    {
        if (dir.x != 0)
        {
            newPos.x += dir.x;
        }
        else if (dir.y != 0)
        {
            newState = BlockState::UP;
            newPos.y = dir.y > 0 ? y + 2 : y - 1;
        }
    }

    this->x = newPos.x;
    this->y = newPos.y;
    this->state = newState;

    if (collide)
    {
        auto positions = this->getPositions();
        if (!level.hasFloorAt(positions.first) || !level.hasFloorAt(positions.second))
        {
            this->x = prevPos.x;
            this->y = prevPos.y;
            this->state = prevState;
            return false;
        }
    }

    return true;
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
    SpriteID sprId = SpriteID::SPR_BLOCK_UP;
    if (state == BlockState::LONG)
        sprId = SpriteID::SPR_BLOCK_LONG;
    if (state == BlockState::WIDE)
        sprId = SpriteID::SPR_BLOCK_WIDE;
    
    int sx, sy;
    toISO(x, y, cellSize, cellSize/2, &sx, &sy);

    Game::DrawSprite(levelX + sx, levelY + sy, sprId);
}
