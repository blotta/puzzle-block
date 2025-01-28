#include "block.hpp"
#include "animation.hpp"


static float animDuration = 0.2f;

SpriteID idle_up_frames[1] = {SPR_BLOCK_UP};
AnimationSprite anim_idle_up = {
    .duration = 1.0f,
    .frameCount = 1,
    .frames = idle_up_frames,
    .loop = true,
};

SpriteID idle_long_frames[1] = {SPR_BLOCK_LONG};
AnimationSprite anim_idle_long = {
    .duration = 1.0f,
    .frameCount = 1,
    .frames = idle_long_frames,
    .loop = true,
};

SpriteID idle_wide_frames[1] = {SPR_BLOCK_WIDE};
AnimationSprite anim_idle_wide = {
    .duration = 1.0f,
    .frameCount = 1,
    .frames = idle_wide_frames,
    .loop = true,
};

SpriteID up_long_frames[3] = {SPR_BLOCK_UP, SPR_BLOCK_UP_LONG_30, SPR_BLOCK_UP_LONG_60};
AnimationSprite anim_up_long = {
    .duration = animDuration,
    .frameCount = 3,
    .frames = up_long_frames,
    .loop = false,
};

SpriteID long_long_frames[3] = {SPR_BLOCK_LONG, SPR_BLOCK_LONG_LONG_30, SPR_BLOCK_LONG_LONG_60};
AnimationSprite anim_long_long = {
    .duration = animDuration,
    .frameCount = 3,
    .frames = long_long_frames,
    .loop = false,
};

SpriteID up_wide_frames[3] = {SPR_BLOCK_UP, SPR_BLOCK_UP_WIDE_30, SPR_BLOCK_UP_WIDE_60};
AnimationSprite anim_up_wide = {
    .duration = animDuration,
    .frameCount = 3,
    .frames = up_wide_frames,
    .loop = false,
};

SpriteID wide_wide_frames[3] = {SPR_BLOCK_WIDE, SPR_BLOCK_WIDE_WIDE_30, SPR_BLOCK_WIDE_WIDE_60};
AnimationSprite anim_wide_wide = {
    .duration = animDuration,
    .frameCount = 3,
    .frames = wide_wide_frames,
    .loop = false,
};

SpriteID long_up_frames[3] = {SPR_BLOCK_LONG_UP_30, SPR_BLOCK_LONG_UP_60, SPR_BLOCK_UP};
AnimationSprite anim_long_up = {
    .duration = animDuration,
    .frameCount = 3,
    .frames = long_up_frames,
    .loop = false,
};

SpriteID wide_up_frames[3] = {SPR_BLOCK_WIDE_UP_30, SPR_BLOCK_WIDE_UP_60, SPR_BLOCK_UP};
AnimationSprite anim_wide_up = {
    .duration = animDuration,
    .frameCount = 3,
    .frames = wide_up_frames,
    .loop = false,
};

std::pair<vec2, vec2> Block::GetBlockPositionsForState(const vec2 &pos, BlockState state)
{
    if (state == BlockState::LONG)
    {
        return std::pair<vec2, vec2>(vec2(pos.x, pos.y), vec2(pos.x, pos.y + 1));
    }

    if (state == BlockState::WIDE)
        return std::pair<vec2, vec2>(vec2(pos.x, pos.y), vec2(pos.x + 1, pos.y));

    return std::pair<vec2,vec2>(vec2(pos.x, pos.y), vec2(pos.x, pos.y));
}

void Block::init(const vec2 &pos, BlockState state)
{
    x = pos.x;
    y = pos.y;
    state = state;
    currTransition = BlockTransition::IDLE;
    animation = &anim_idle_up;
    if (state == BlockState::LONG)
        animation = &anim_idle_long;
    else if (state == BlockState::WIDE)
        animation = &anim_idle_wide;

    moveIntent.oldPos = vec2(x, y);
    moveIntent.newPos = vec2(x, y);
    moveIntent.moved = false;
    moveIntent.transition = BlockTransition::IDLE;
}

bool Block::move(const vec2 &dir, const Level &level, bool collide)
{
    if (currTransition != BlockTransition::IDLE)
    {
        if (animation->isDone())
        {
            SDL_Log("animation done\n");
            x = moveIntent.newPos.x;
            y = moveIntent.newPos.y;
            state = moveIntent.newState;

            moveIntent.oldPos = moveIntent.newPos;
            moveIntent.oldState = moveIntent.newState;
            moveIntent.moved = false;
            moveIntent.transition = BlockTransition::IDLE;

            currTransition = BlockTransition::IDLE;
            if (state == BlockState::UP)
                animation = &anim_idle_up;
            else if (state == BlockState::LONG)
                animation = &anim_idle_long;
            else if (state == BlockState::WIDE)
                animation = &anim_idle_wide;
            
            animation->start();
            return true;
        }
        return false;
    }

    if (dir.x == 0 && dir.y == 0)
        return false;

    this->updateMovementIntent(dir);

    if (collide)
    {
        auto positions = Block::GetBlockPositionsForState(moveIntent.newPos, moveIntent.newState);
        if (!level.hasFloorAt(positions.first) || !level.hasFloorAt(positions.second))
        {
            moveIntent.moved = false;
            moveIntent.newPos = moveIntent.oldPos;
            moveIntent.newState = moveIntent.oldState;
            return false;
        }
    }


    if (moveIntent.moved)
    {
        switch (moveIntent.transition)
        {
            case BlockTransition::UP_LONG_FORWARD:
                animation = &anim_up_long;
                animation->reverse = false;
                break;
            case BlockTransition::LONG_UP_BACKWARD:
                animation = &anim_up_long;
                animation->reverse = true;
                y = moveIntent.newPos.y;
                break;
            case BlockTransition::UP_LONG_BACKWARD:
                animation = &anim_long_up;
                animation->reverse = true;
                break;
            case BlockTransition::LONG_UP_FORWARD:
                animation = &anim_long_up;
                animation->reverse = false;
                y = moveIntent.newPos.y;
                break;

            case BlockTransition::UP_WIDE_RIGHT:
                animation = &anim_up_wide;
                animation->reverse = false;
                break;
            case BlockTransition::WIDE_UP_LEFT:
                animation = &anim_up_wide;
                animation->reverse = true;
                x = moveIntent.newPos.x;
                break;
            case BlockTransition::UP_WIDE_LEFT:
                animation = &anim_wide_up;
                animation->reverse = true;
                break;
            case BlockTransition::WIDE_UP_RIGHT:
                animation = &anim_wide_up;
                animation->reverse = false;
                x = moveIntent.newPos.x;
                break;

            case BlockTransition::LONG_LONG_RIGHT:
                animation = &anim_long_long;
                animation->reverse = false;
                break;
            case BlockTransition::LONG_LONG_LEFT:
                animation = &anim_long_long;
                animation->reverse = true;
                x = moveIntent.newPos.x;
                break;

            case BlockTransition::WIDE_WIDE_FORWARD:
                animation = &anim_wide_wide;
                animation->reverse = false;
                break;
            case BlockTransition::WIDE_WIDE_BACKWARD:
                animation = &anim_wide_wide;
                animation->reverse = true;
                y = moveIntent.newPos.y;
                break;
            default:
                break;
        }
        currTransition = moveIntent.transition;
        animation->start();
    }

    return false;
}


void Block::updateMovementIntent(const vec2& dir)
{
    auto& mv = this->moveIntent;
    mv.oldPos.x = this->x;
    mv.oldPos.y = this->y;
    mv.oldState = this->state;
    mv.newPos.x = mv.oldPos.x;
    mv.newPos.y = mv.oldPos.y;
    mv.newState = mv.oldState;
    mv.transition = BlockTransition::IDLE;

    if (dir.x != 0 || dir.y != 0)
        mv.moved = true;
    else
        return;

    if (this->state == BlockState::UP)
    {
        if (dir.x != 0)
        {
            mv.newState = BlockState::WIDE;
            mv.newPos.x = dir.x > 0 ? this->x + 1 : this->x - 2;
            mv.transition = dir.x > 0 ? BlockTransition::UP_WIDE_RIGHT : BlockTransition::UP_WIDE_LEFT;
        }
        else if (dir.y != 0)
        {
            mv.newState = BlockState::LONG;
            mv.newPos.y = dir.y > 0 ? this->y + 1 : this->y - 2;
            mv.transition = dir.y > 0 ? BlockTransition::UP_LONG_BACKWARD : BlockTransition::UP_LONG_FORWARD;
        }
    }
    else if (this->state == BlockState::WIDE)
    {
        if (dir.x != 0)
        {
            mv.newState = BlockState::UP;
            mv.newPos.x = dir.x > 0 ? this->x + 2 : this->x - 1;
            mv.transition = dir.x > 0 ? BlockTransition::WIDE_UP_RIGHT : BlockTransition::WIDE_UP_LEFT;
        }
        else if (dir.y != 0)
        {
            mv.newPos.y += dir.y;
            mv.transition = dir.y > 0 ? BlockTransition::WIDE_WIDE_BACKWARD : BlockTransition::WIDE_WIDE_FORWARD;
        }
    }
    else if (this->state == BlockState::LONG)
    {
        if (dir.x != 0)
        {
            mv.newPos.x += dir.x;
            mv.transition = dir.x > 0 ? BlockTransition::LONG_LONG_RIGHT : BlockTransition::LONG_LONG_LEFT;
        }
        else if (dir.y != 0)
        {
            mv.newState = BlockState::UP;
            mv.newPos.y = dir.y > 0 ? this->y + 2 : this->y - 1;
            mv.transition = dir.y > 0 ? BlockTransition::LONG_UP_BACKWARD : BlockTransition::LONG_UP_FORWARD;
        }
    }
}

std::pair<vec2, vec2> Block::getPositions()
{
    return Block::GetBlockPositionsForState(vec2(this->x, this->y), this->state);
}

void Block::draw(int levelX, int levelY, int cellSize)
{
    SpriteID sprId = animation->tick();

    int sx, sy;
    IsoToWorld(x, y, cellSize, cellSize/2, &sx, &sy);

    Game::DrawSprite(levelX + sx, levelY + sy, sprId);
}
