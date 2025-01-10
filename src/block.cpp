#include "block.hpp"
#include "animation.hpp"


SpriteID idle_up_frames[1] = {SPR_BLOCK_UP};
AnimationSprite anim_idle_up = {
    .duration = 1.0f,
    .frameCount = 1,
    .frames = idle_up_frames,
    .loop = false,
};

SpriteID idle_long_frames[1] = {SPR_BLOCK_LONG};
AnimationSprite anim_idle_long = {
    .duration = 1.0f,
    .frameCount = 1,
    .frames = idle_long_frames,
    .loop = false,
};

SpriteID idle_wide_frames[1] = {SPR_BLOCK_WIDE};
AnimationSprite anim_idle_wide = {
    .duration = 1.0f,
    .frameCount = 1,
    .frames = idle_wide_frames,
    .loop = false,
};

SpriteID up_long_frames[3] = {SPR_BLOCK_UP, SPR_BLOCK_UP_LONG_30, SPR_BLOCK_UP_LONG_60};
AnimationSprite anim_up_long = {
    .duration = 2.0f,
    .frameCount = 3,
    .frames = up_long_frames,
    .loop = true,
};

void Block::init(const vec2 &pos, BlockState state)
{
    x = pos.x;
    y = pos.y;
    state = state;
    animState = BlockAnimationState::IDLE;
    animation = &anim_idle_up;
    if (state == BlockState::LONG)
        animation = &anim_idle_long;
    else if (state == BlockState::WIDE)
        animation = &anim_idle_wide;
}

bool Block::move(const vec2 &dir, const Level &level, bool collide)
{
    if (animState != BlockAnimationState::IDLE)
    {
        if (animation->isDone())
            animState = BlockAnimationState::IDLE;
        return false;
    }

    if (dir.x == 0 && dir.y == 0)
        return false;

    vec2 prevPos(x,y);
    BlockState prevState = this->state;

    vec2 newPos(x,y);
    BlockState newState = this->state;
    
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

    if (state == BlockState::UP)
        animation = &anim_idle_up;
    else if (state == BlockState::LONG)
        animation = &anim_idle_long;
    else if (state == BlockState::WIDE)
        animation = &anim_idle_wide;
    
    animation->start();

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
    SpriteID sprId = animation->tick();

    int sx, sy;
    toISO(x, y, cellSize, cellSize/2, &sx, &sy);

    Game::DrawSprite(levelX + sx, levelY + sy, sprId);
}
