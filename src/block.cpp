#include "block.hpp"

#include "asset.hpp"

Block::Block(Game *game)
    :game(game)
{
    pSpriteSheet = game->mAsset.getTexture("assets/images/spritesheet.png");
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
    SpriteID sprId = SpriteID::BLOCK_UP;
    if (state == BlockState::LONG)
        sprId = SpriteID::BLOCK_LONG;
    if (state == BlockState::WIDE)
        sprId = SpriteID::BLOCK_WIDE;
    
    Sprite* spr = &SPRITES[sprId];

    SDL_Rect src = {
        spr->tx,
        spr->ty,
        spr->tw,
        spr->th
    };

    int scale = 1;

    int sx, sy;
    toISO(x, y, cellSize, cellSize/2, &sx, &sy);

    SDL_Rect dest = {
        levelX + sx - spr->originX * scale,
        levelY + sy - spr->originY * scale,
        spr->tw * scale,
        spr->th * scale
    };

    SDL_RenderCopy(rend, pSpriteSheet->get(), &src, &dest);
}
