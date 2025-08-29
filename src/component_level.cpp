#include "component_level.hpp"
#include "util.hpp"

static const int cellSize = 64;

LevelComponent::LevelComponent(int lvlIdx, const LevelData& ld)
{
    mVisualState = LevelVisualState::IDLE;
    this->load(lvlIdx, ld);
}

void LevelComponent::load(int lvlIdx, const LevelData& ld)
{
    this->lvlIdx = lvlIdx;
    this->mModel.load(ld);
    this->animRise.stop();

    int baseStartHeight = 400;

    for (int i = 0; i < mModel.cols; i++)
    {
        for (int j = 0; j < mModel.rows; j++)
        {
            int width = randomNeg1to1() * Game::ScreenWidth() / 3;
            int height = baseStartHeight + randomNeg1to1() * 100;
            float endTime = 1.0f - (random01() * 0.5f);

            auto& prop = tileAnims[j][i];
            prop.interpolationType = InterpolationType::EASE_OUT_BACK;
            prop.addKeyframe(0.f, {width, height});
            prop.addKeyframe(endTime, {0, 0});
        }
    }
}

void LevelComponent::startRise(std::function<void()> onComplete)
{
    mVisualState = LevelVisualState::RISING;
    animRise.onComplete = onComplete;
    animRise.start();
}

void LevelComponent::init()
{
    Log::debug("levelComponent init");
    this->transform = owner->getComponent<CTransform>();
    this->animRise.duration = 2.f;
}

void LevelComponent::update(float dt)
{
    if (mVisualState == LevelVisualState::IDLE)
    {
        if (block && block->moved)
        {
            auto positions = block->currSim.getPositions();
            mModel.checkAndTriggerSwitches(positions.first, positions.second);

            if (mModel.isCleared(positions.first, positions.second))
            {
                if (onLevelCleared)
                    onLevelCleared();
            }
        }
    }
    else if (mVisualState == LevelVisualState::RISING)
    {
        animRise.update(dt);
        if (animRise.getProgress() == 1.f)
        {
            this->animRise.stop();
            this->mVisualState = LevelVisualState::IDLE;
        }
    }
}

void LevelComponent::draw()
{
    for (int i = 0; i < mModel.cols; i++)
    {
        for (int j = 0; j < mModel.rows; j++)
        {

            if (mModel.grid[j][i] == CellType::EMPTY)
                continue;

            SpriteID sprId = SpriteID::SPR_FLOOR;

            switch (mModel.grid[j][i])
            {
            case CellType::START:
                sprId = SpriteID::SPR_FLOOR_START;
                break;
            case CellType::FINISH:
                sprId = SpriteID::SPR_FLOOR_FINISH;
                break;
            case CellType::THIN:
                sprId = SpriteID::SPR_FLOOR_THIN;
                break;
            default:
                sprId = SpriteID::SPR_FLOOR;
                break;
            }

            int sx;
            int sy;
            IsoToWorld(i, j, cellSize, cellSize / 2, &sx, &sy);

            vec2 posDiff = {0, 0};

            if (mVisualState == LevelVisualState::RISING)
            {
                posDiff = tileAnims[j][i].evaluate(animRise.getProgress());
            }

            Game::DrawSprite(this->transform->pos.x + sx + posDiff.x, this->transform->pos.y + sy + posDiff.y, sprId);

            for (int sidx = 0; sidx < this->mModel.switchCount; sidx++)
            {
                auto& sw = this->mModel.switches[sidx];
                if (sw.x != i || sw.y != j)
                    continue;

                int sx;
                int sy;
                IsoToWorld(sw.x, sw.y, cellSize, cellSize / 2, &sx, &sy);

                Game::DrawSprite(this->transform->pos.x + sx + posDiff.x, this->transform->pos.y + sy + posDiff.y,
                                 sw.on ? SpriteID::SPR_SWITCH_ON : SpriteID::SPR_SWITCH_OFF);
            }
        }
    }
}