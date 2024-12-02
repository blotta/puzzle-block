#include "block.hpp"

static IsoSprite isoSprUp = {
    .tx = 0,
    .ty = 64,
    .tw = 64,
    .th = 96,
    .heightOffset = 64,
    .widthOffset = 0
};

static IsoSprite isoSprLong = {
    .tx = 64,
    .ty = 80,
    .tw = 96,
    .th = 80,
    .heightOffset = 32,
    .widthOffset = 32
};

static IsoSprite isoSprWide = {
    .tx = 160,
    .ty = 80,
    .tw = 96,
    .th = 80,
    .heightOffset = 32,
    .widthOffset = 0
};

Block::Block(Game *game)
    :game(game)
{
    pSpriteSheet = game->getTexture("spritesheet");
}

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

    SDL_Log("block %d %d\n", x, y);
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

void Block::draw(SDL_Renderer *rend, int levelX, int levelY, int cellSize)
{
    SDL_SetRenderDrawColor(rend, 200, 100, 100, 255);
    SDL_Rect blockRect = {
        levelX + this->x * cellSize,
        levelY + this->y * cellSize,
        cellSize,
        cellSize
    };
    if (state == BlockState::WIDE)
        blockRect.w *= 2;
    if (state == BlockState::LONG)
        blockRect.h *= 2;
    SDL_RenderFillRect(rend, &blockRect);
}

void Block::drawISO(SDL_Renderer *rend, int levelX, int levelY, int cellSize)
{
    IsoSprite* bs = &isoSprUp;
    if (state == BlockState::LONG)
        bs = &isoSprLong;
    if (state == BlockState::WIDE)
        bs = &isoSprWide;

    SDL_Rect src = {
        bs->tx,
        bs->ty,
        bs->tw,
        bs->th
    };

    int scale = cellSize / isoSprUp.tw;

    int sx, sy;
    toISO(x, y, cellSize, cellSize/2, &sx, &sy);

    SDL_Rect dest = {
        levelX + sx - bs->widthOffset * scale,
        levelY + sy - bs->heightOffset * scale,
        bs->tw * scale,
        bs->th * scale
    };

    SDL_RenderCopy(rend, pSpriteSheet, &src, &dest);
}
