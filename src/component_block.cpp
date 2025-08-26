#include "component_block.hpp"
#include "input_manager.hpp"

static const int cellSize = 64;

std::unordered_map<BlockVisualTransition, BlockVisualTransitionData> BLOCK_FRAMES = {{
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

void BlockComponent::startFall()
{
    this->vState = BlockVisualState::FALLING_LEVEL_START;
    this->animFallStart.start();
}

void BlockComponent::startFall(std::function<void()> onComplete)
{
    this->animFallStart.onComplete = onComplete;
    this->startFall();
}

void BlockComponent::startFly(std::function<void()> onComplete)
{
    this->vState = BlockVisualState::FLYING_LEVEL_END;
    this->animFlyEnd.onComplete = onComplete;
    this->animFlyEnd.start();
}

void BlockComponent::init()
{
    Log::debug("blockComponent init");
    transform = owner->getComponent<CTransform>();

    vec2 pos;
    if (level)
        pos = level->mModel.getStartPos();

    this->currSim.x = pos.x;
    this->currSim.y = pos.y;
    this->currSim.state = BlockState::UP;
    this->nextSim = this->currSim;
    this->vState = BlockVisualState::IDLE;
    this->anim.duration = 0.15f;
    this->anim.stop();
    this->animProp.addKeyframesEvenly(BLOCK_FRAMES.at(BlockVisualTransition::IDLE_UP).frames);
    this->anim.events.clear();
    this->anim.addEvent(1.f, [this]() {
        Game::PlaySound("assets/sfx/block_move.ogg");
        this->hitFloor = true;
    });

    this->animFallStart.duration = 0.5f;
    this->animFallStart.stop();
    this->animFallStartProp.interpolationType = InterpolationType::EASE_IN;
    this->animFallStartProp.addKeyframe(0.f, 1.f);
    this->animFallStartProp.addKeyframe(1.f, 0.0f);
    this->animFallStart.events.clear();
    this->animFallStart.addEvent(1.f, [this]() {
        Game::PlaySound("assets/sfx/snd_gunshot1.ogg");
        this->hitFloor = true;
    });

    this->animFlyEnd.duration = 0.5f;
    this->animFlyEnd.mode = AnimationPlayMode::ONCE;
    this->animFlyEnd.stop();
    this->animFlyEndProp.interpolationType = InterpolationType::EASE_IN;
    this->animFlyEndProp.addKeyframe(0.f, 0.f);
    this->animFlyEndProp.addKeyframe(1.f, 1.0f);
    this->animFlyEnd.events.clear();
    this->animFlyEnd.addEvent(0.f, []() { Game::PlaySound("assets/sfx/uff.ogg"); });

    this->particleSystem.startColor = {180, 150, 150, 255};
    this->particleSystem.endColor = this->particleSystem.startColor;
    this->particleSystem.endColor.a = 0;
    this->particleSystem.vx = .5f;
    this->particleSystem.vy = .2f;
    this->particleSystem.gravity = 0.f;
    this->particleSystem.minLife = 0.3f;
    this->particleSystem.maxLife = 0.5f;
}

void BlockComponent::update(float dt)
{
    this->moved = false;
    if (vState == BlockVisualState::IDLE)
    {
        this->nextSim = this->currSim;
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
                if (!level->mModel.hasFloorAt(positions.first) || !level->mModel.hasFloorAt(positions.second))
                {
                    nextSim = currSim;
                    return;
                }
            }

            transitionData = BLOCK_FRAMES.at(BlockVisual::getTransition(currSim, nextSim));
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
        }
    }

    this->particleSystem.update(dt);
}

void BlockComponent::draw()
{
    if (vState == BlockVisualState::IDLE)
    {
        // draw idle state
        SpriteID sprId = currSim.state == BlockState::UP     ? SPR_BLOCK_UP
                         : currSim.state == BlockState::LONG ? SPR_BLOCK_LONG
                                                             : SPR_BLOCK_WIDE;
        int sx, sy;
        IsoToWorld(currSim.x, currSim.y, cellSize, cellSize / 2, &sx, &sy);
        Game::DrawSprite(this->transform->pos.x + sx, this->transform->pos.y + sy, sprId);
    }
    else if (vState == BlockVisualState::MOVING)
    {
        // draw moving state
        SpriteID sprId = animProp.evaluate(anim.getProgress());
        int sx, sy;
        BlockSim& sim = transitionData.useCurrSim ? currSim : nextSim;
        IsoToWorld(sim.x, sim.y, cellSize, cellSize / 2, &sx, &sy);
        Game::DrawSprite(this->transform->pos.x + sx, this->transform->pos.y + sy, sprId);
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
        float height = (sy + 400) * heightPerc;
        Game::DrawSprite(this->transform->pos.x + sx, this->transform->pos.y + sy - height, sprId);
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
        float height = (sy + 400) * heightPerc;
        Game::DrawSprite(this->transform->pos.x + sx, this->transform->pos.y + sy - height, sprId);
    }

    if (hitFloor)
    {
        this->hitFloor = false;

        int bx, by;
        IsoToWorld(currSim.x, currSim.y, cellSize, cellSize / 2, &bx, &by);

        const int quarterCS = cellSize / 4;
        const int halfCS = cellSize / 2;
        const int threeForthsCS = cellSize * 3 / 4;
        const int oneAndHalfCS = cellSize * 1.5f;

        // currSim.state == BlockState::UP
        float x1 = bx + 0;
        float y1 = by + quarterCS;
        float x2 = bx + halfCS;
        float y2 = by + halfCS;
        float x3 = bx + cellSize;
        float y3 = by + quarterCS;
        if (currSim.state == BlockState::LONG)
        {
            x1 = bx - halfCS;
            y1 = by + halfCS;
            x2 = bx + 0;
            y2 = by + threeForthsCS;
            x3 = bx + cellSize;
            y3 = by + quarterCS;
        }
        else if (currSim.state == BlockState::WIDE)
        {
            x1 = bx + 0;
            y1 = by + quarterCS;
            x2 = bx + cellSize;
            y2 = by + threeForthsCS;
            x3 = bx + oneAndHalfCS;
            y3 = by + halfCS;
        }

        particleSystem.emitLine(x1, y1, x2, y2, 50);
        particleSystem.emitLine(x2, y2, x3, y3, 50);
    }

    particleSystem.draw(this->transform->pos.x, this->transform->pos.y);
}