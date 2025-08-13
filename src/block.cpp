#include "block.hpp"
#include "animation.hpp"
#include "input_manager.hpp"
#include "log.hpp"

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

std::unordered_map<BlockVisualTransition, BlockVisualTransitionData> FRAMES = {{
    {BlockVisualTransition::IDLE_UP, {{SPR_BLOCK_UP, SPR_BLOCK_UP}, true}},
    {BlockVisualTransition::IDLE_WIDE, {{SPR_BLOCK_WIDE, SPR_BLOCK_WIDE}, true}},
    {BlockVisualTransition::IDLE_LONG, {{SPR_BLOCK_LONG, SPR_BLOCK_LONG}, true}},

    {BlockVisualTransition::UP_LONG_FORWARD, {{SPR_BLOCK_UP_LONG_30, SPR_BLOCK_UP_LONG_60}, true}},
    {BlockVisualTransition::LONG_UP_BACKWARD, {{SPR_BLOCK_UP_LONG_60, SPR_BLOCK_UP_LONG_30}, false}},

    {BlockVisualTransition::UP_LONG_BACKWARD, {{SPR_BLOCK_LONG_UP_60, SPR_BLOCK_LONG_UP_30}, true}},
    {BlockVisualTransition::LONG_UP_FORWARD, {{SPR_BLOCK_LONG_UP_30, SPR_BLOCK_LONG_UP_60}, false}},

    {BlockVisualTransition::UP_WIDE_RIGHT, {{SPR_BLOCK_UP_WIDE_30, SPR_BLOCK_UP_WIDE_60}, true}},
    {BlockVisualTransition::WIDE_UP_LEFT, {{SPR_BLOCK_UP_WIDE_60, SPR_BLOCK_UP_WIDE_30}, false}},

    {BlockVisualTransition::UP_WIDE_LEFT, {{SPR_BLOCK_WIDE_UP_60, SPR_BLOCK_WIDE_UP_30}, true}},
    {BlockVisualTransition::WIDE_UP_RIGHT, {{SPR_BLOCK_WIDE_UP_30, SPR_BLOCK_WIDE_UP_60}, false}},

    {BlockVisualTransition::LONG_LONG_RIGHT, {{SPR_BLOCK_LONG_LONG_30, SPR_BLOCK_LONG_LONG_60}, true}},
    {BlockVisualTransition::LONG_LONG_LEFT, {{SPR_BLOCK_LONG_LONG_60, SPR_BLOCK_LONG_LONG_30}, false}},

    {BlockVisualTransition::WIDE_WIDE_FORWARD, {{SPR_BLOCK_WIDE_WIDE_30, SPR_BLOCK_WIDE_WIDE_60}, true}},
    {BlockVisualTransition::WIDE_WIDE_BACKWARD, {{SPR_BLOCK_WIDE_WIDE_60, SPR_BLOCK_WIDE_WIDE_30}, false}},
}};

void BlockVisual::init(const vec2& pos, BlockState state)
{
    this->currSim.x = pos.x;
    this->currSim.y = pos.y;
    this->currSim.state = state;
    this->nextSim = this->currSim;
    this->vState = BlockVisualState::IDLE;
    this->anim.duration = 0.15f;
    this->anim.stop();
    this->animProp.addKeyframesEvenly(FRAMES.at(BlockVisualTransition::IDLE_UP).frames);
    this->anim.addEvent(1.f, []() { Game::PlaySound("assets/sfx/block_move.ogg"); });

    this->animFallStart.duration = 0.5f;
    this->animFallStart.stop();
    this->animFallStartProp.interpolationType = InterpolationType::EASE_IN;
    this->animFallStartProp.addKeyframe(0.f, 1.f);
    this->animFallStartProp.addKeyframe(1.f, 0.0f);
    this->animFallStart.addEvent(1.f, []() { Game::PlaySound("assets/sfx/snd_gunshot1.ogg"); });

    this->animFlyEnd.duration = 0.5f;
    this->animFlyEnd.mode = AnimationPlayMode::ONCE;
    this->animFlyEnd.stop();
    this->animFlyEndProp.interpolationType = InterpolationType::EASE_IN;
    this->animFlyEndProp.addKeyframe(0.f, 0.f);
    this->animFlyEndProp.addKeyframe(1.f, 1.0f);
    this->animFlyEnd.addEvent(0.f, []() { Game::PlaySound("assets/sfx/uff.ogg"); });
}

void BlockVisual::update(float dt)
{
    if (vState == BlockVisualState::IDLE)
    {
        this->nextSim = this->currSim;
        this->moved = false;

        auto moveDir = vec2(0, 0);
        if (Input::JustPressed(SDL_SCANCODE_UP))
            moveDir = vec2(0, -1);
        if (Input::JustPressed(SDL_SCANCODE_DOWN))
            moveDir = vec2(0, 1);
        if (Input::JustPressed(SDL_SCANCODE_LEFT))
            moveDir = vec2(-1, 0);
        if (Input::JustPressed(SDL_SCANCODE_RIGHT))
            moveDir = vec2(1, 0);

        if (moveDir.x != 0 || moveDir.y != 0)
        {
            nextSim.move(moveDir);

            if (level != nullptr) // collide
            {
                auto positions = nextSim.getPositions();
                if (!level->hasFloorAt(positions.first) || !level->hasFloorAt(positions.second))
                {
                    nextSim = currSim;
                    return;
                }
            }

            transitionData = FRAMES.at(getTransition(currSim, nextSim));
            animProp.addKeyframesEvenly(transitionData.frames);
            this->vState = BlockVisualState::MOVING;
            this->anim.start();
        }
    }
    else if (vState == BlockVisualState::MOVING)
    {
        this->anim.update(dt);
        if (this->anim.time >= this->anim.duration)
        {
            this->vState = BlockVisualState::IDLE;
            this->anim.stop();
            this->currSim = this->nextSim;
            moved = true;
        }
    }
    else if (vState == BlockVisualState::FALLING_LEVEL_START)
    {
        this->animFallStart.update(dt);
        if (this->animFallStart.getProgress() >= 1.0f)
        {
            this->animFallStart.stop();
            this->vState = BlockVisualState::IDLE;
        }
    }
    else if (vState == BlockVisualState::FLYING_LEVEL_END)
    {
        this->animFlyEnd.update(dt);
        if (this->animFlyEnd.getProgress() >= 1.0f)
        {
            this->animFlyEnd.stop();
            // this->vState = BlockVisualState::IDLE;
        }
    }
}

void BlockVisual::draw(int levelX, int levelY, int cellSize)
{
    if (vState == BlockVisualState::IDLE)
    {
        // draw idle state
        SpriteID sprId = currSim.state == BlockState::UP     ? SPR_BLOCK_UP
                         : currSim.state == BlockState::LONG ? SPR_BLOCK_LONG
                                                             : SPR_BLOCK_WIDE;
        int sx, sy;
        IsoToWorld(currSim.x, currSim.y, cellSize, cellSize / 2, &sx, &sy);
        Game::DrawSprite(levelX + sx, levelY + sy, sprId);
    }
    else if (vState == BlockVisualState::MOVING)
    {
        // draw moving state
        SpriteID sprId = animProp.evaluate(anim.getProgress());
        int sx, sy;
        BlockSim& sim = transitionData.useCurrSim ? currSim : nextSim;
        IsoToWorld(sim.x, sim.y, cellSize, cellSize / 2, &sx, &sy);
        Game::DrawSprite(levelX + sx, levelY + sy, sprId);
    }
    else if (vState == BlockVisualState::FALLING_LEVEL_START)
    {
        // draw idle state
        SpriteID sprId = currSim.state == BlockState::UP     ? SPR_BLOCK_UP
                         : currSim.state == BlockState::LONG ? SPR_BLOCK_LONG
                                                             : SPR_BLOCK_WIDE;
        int sx, sy;
        IsoToWorld(currSim.x, currSim.y, cellSize, cellSize / 2, &sx, &sy);

        auto heightPerc = animFallStartProp.evaluate(animFallStart.getProgress());
        float height = (levelY + sy + 200) * heightPerc;
        Game::DrawSprite(levelX + sx, levelY + sy - height, sprId);
    }
    else if (vState == BlockVisualState::FLYING_LEVEL_END)
    {
        // draw idle state
        SpriteID sprId = currSim.state == BlockState::UP     ? SPR_BLOCK_UP
                         : currSim.state == BlockState::LONG ? SPR_BLOCK_LONG
                                                             : SPR_BLOCK_WIDE;
        int sx, sy;
        IsoToWorld(currSim.x, currSim.y, cellSize, cellSize / 2, &sx, &sy);

        auto heightPerc = animFlyEndProp.evaluate(animFlyEnd.getProgress());
        float height = (levelY + sy + 200) * heightPerc;
        // Log::debug("heightPerc: %f", heightPerc);
        Game::DrawSprite(levelX + sx, levelY + sy - height, sprId);
    }
}

void BlockVisual::startFall()
{
    this->vState = BlockVisualState::FALLING_LEVEL_START;
    this->animFallStart.start();
}

void BlockVisual::startFly(const std::function<void()>& onComplete)
{
    this->vState = BlockVisualState::FLYING_LEVEL_END;
    this->animFlyEnd.onComplete = onComplete;
    this->animFlyEnd.start();
}

BlockVisualTransition BlockVisual::getTransition(const BlockSim& curr, const BlockSim& next)
{
    if (curr.state == BlockState::UP)
    {
        if (next.state == BlockState::LONG)
        {
            if (next.y > curr.y)
                return BlockVisualTransition::UP_LONG_BACKWARD;
            else
                return BlockVisualTransition::UP_LONG_FORWARD;
        }
        if (next.state == BlockState::WIDE)
        {
            if (next.x > curr.x)
                return BlockVisualTransition::UP_WIDE_RIGHT;
            else
                return BlockVisualTransition::UP_WIDE_LEFT;
        }
        if (next.state == BlockState::UP)
            return BlockVisualTransition::IDLE_UP;
    }
    if (curr.state == BlockState::LONG)
    {
        if (next.state == BlockState::UP)
        {
            if (next.y > curr.y)
                return BlockVisualTransition::LONG_UP_BACKWARD;
            else
                return BlockVisualTransition::LONG_UP_FORWARD;
        }
        if (next.state == BlockState::LONG)
        {
            if (next.x > curr.x)
                return BlockVisualTransition::LONG_LONG_RIGHT;
            else
                return BlockVisualTransition::LONG_LONG_LEFT;
        }
    }
    if (curr.state == BlockState::WIDE)
    {
        if (next.state == BlockState::UP)
        {
            if (next.x > curr.x)
                return BlockVisualTransition::WIDE_UP_RIGHT;
            else
                return BlockVisualTransition::WIDE_UP_LEFT;
        }
        if (next.state == BlockState::WIDE)
        {
            if (next.y > curr.y)
                return BlockVisualTransition::WIDE_WIDE_BACKWARD;
            else
                return BlockVisualTransition::WIDE_WIDE_FORWARD;
        }
    }

    // should not get here
    Log::error("Invalid block visual transition from %d to %d\n", (int)curr.state, (int)next.state);
    return BlockVisualTransition::IDLE_UP;
}
