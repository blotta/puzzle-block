#include "block.hpp"
#include "animation.hpp"
#include "input_manager.hpp"
#include "log.hpp"

static const int cellSize = 64;

std::pair<vec2, vec2> BlockSim::GetBlockPositionsForState(const vec2& pos, BlockState state)
{
    if (state == BlockState::LONG)
    {
        return std::pair<vec2, vec2>(vec2(pos.x, pos.y), vec2(pos.x, pos.y + 1));
    }

    if (state == BlockState::WIDE)
        return std::pair<vec2, vec2>(vec2(pos.x, pos.y), vec2(pos.x + 1, pos.y));

    return std::pair<vec2, vec2>(vec2(pos.x, pos.y), vec2(pos.x, pos.y));
}

void BlockSim::move(const vec2& dir)
{
    if (dir.x == 0 && dir.y == 0)
        return;

    switch (state)
    {
    case BlockState::UP:
        if (dir.x != 0)
        {
            x += (dir.x > 0) ? 1 : -2;
            state = BlockState::WIDE;
        }
        else
        {
            y += (dir.y > 0) ? 1 : -2;
            state = BlockState::LONG;
        }
        break;

    case BlockState::WIDE:
        if (dir.x != 0)
        {
            x += (dir.x > 0) ? 2 : -1;
            state = BlockState::UP;
        }
        else
        {
            y += dir.y;
        }
        break;

    case BlockState::LONG:
        if (dir.y != 0)
        {
            y += (dir.y > 0) ? 2 : -1;
            state = BlockState::UP;
        }
        else
        {
            x += dir.x;
        }
        break;
    }
}

std::pair<vec2, vec2> BlockSim::getPositions()
{
    if (state == BlockState::LONG)
        return {{x, y}, {x, y + 1}};
    if (state == BlockState::WIDE)
        return {{x, y}, {x + 1, y}};
    return {{x, y}, {x, y}};
}
