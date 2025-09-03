#include "component_leveledit.hpp"

static const int cellSize = 64;

LevelEditComponent::LevelEditComponent(int lvlIdx, const LevelData& ld)
{
    this->load(lvlIdx, ld);
}

void LevelEditComponent::load(int lvlIdx, const LevelData& ld)
{
    this->lvlIdx = lvlIdx;
    this->mModel.load(ld);
    this->mModel.cols = MAX_GRID_SIZE;
    this->mModel.rows = MAX_GRID_SIZE;
    this->mModel.centerCells();
    this->rect = this->mModel.rect();
    this->modified = false;
    if (onChange)
        onChange();
}

void LevelEditComponent::save(bool newLevel, bool saveToFile)
{
    Level m = this->mModel;
    m.trim();
    LevelData ld = {};
    m.toLevelData(&ld);

    if (newLevel)
    {
        lvlIdx = Game::AddLevelData(ld);
        Game::SetState("curr_level", std::to_string(lvlIdx));
    }
    else
    {
        Game::SaveLevelData(ld, lvlIdx);
    }

    if (saveToFile)
    {
#ifndef __EMSCRIPTEN__
        char fileName[50] = {};
        sprintf(fileName, "assets-build/levels/%d.txt", lvlIdx + 1);
        FILE* f = fopen(fileName, "w");
        ld.print(f);
        fclose(f);
#endif
    }

    this->load(lvlIdx, ld);
}

int LevelEditComponent::cols() const
{
    return rect.w;
}

int LevelEditComponent::rows() const
{
    return rect.h;
}

bool LevelEditComponent::isModified() const
{
    return modified;
}

bool LevelEditComponent::isValid() const
{
    return this->mModel.isValid();
}

vec2 LevelEditComponent::getStartPos() const
{
    return this->mModel.getStartPos();
}

void LevelEditComponent::toggleFloor(const vec2& pos)
{
    this->mModel.toggleFloor(pos);
    this->mModel.centerCells();
    this->modified = true;
    this->justModified = true;
}

void LevelEditComponent::toggleSpecialFloor(const vec2& pos)
{
    this->mModel.toggleSpecialFloor(pos);
    this->mModel.centerCells();
    this->modified = true;
    this->justModified = true;
}

bool LevelEditComponent::isValidPos(const vec2& pos) const
{
    return this->mModel.isValidPos(pos);
}

CellType LevelEditComponent::cellAt(const vec2& pos) const
{
    return this->mModel.cellAt(pos);
}

bool LevelEditComponent::hasSwitchAt(const vec2& pos, LevelSwitch** sw)
{
    return this->mModel.hasSwitchAt(pos, sw);
}

void LevelEditComponent::removeSwitch(const vec2& pos)
{
    this->modified = true;
    this->justModified = true;
    this->mModel.removeSwitch(pos);
    this->mModel.centerCells();
}

void LevelEditComponent::addSwitch(LevelSwitch sw)
{
    this->modified = true;
    this->justModified = true;
    this->mModel.addSwitch(sw);
    this->mModel.centerCells();
}


void LevelEditComponent::init()
{
    transform = owner->getComponent<CTransform>();
}

void LevelEditComponent::update(float dt)
{
    if (justModified)
    {
        this->justModified = false;
        this->rect = this->mModel.rect();
        if (onChange)
            onChange();
    }
}

void LevelEditComponent::draw()
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

            Game::DrawSprite(this->transform->pos.x + sx, this->transform->pos.y + sy, sprId);

            for (int sidx = 0; sidx < this->mModel.switchCount; sidx++)
            {
                auto& sw = this->mModel.switches[sidx];
                if (sw.x != i || sw.y != j)
                    continue;

                int sx;
                int sy;
                IsoToWorld(sw.x, sw.y, cellSize, cellSize / 2, &sx, &sy);

                Game::DrawSprite(this->transform->pos.x + sx, this->transform->pos.y + sy,
                                 sw.on ? SpriteID::SPR_SWITCH_ON : SpriteID::SPR_SWITCH_OFF);

                // level switches lines
                // LevelSwitch& sw = this->mModel.switches[i];
                SDL_Color switchLineColor = {255, 180, 0, 255};
                if (sw.on)
                    switchLineColor = {180, 255, 0, 255};
                int swX, swY, swfX, swfY;
                IsoToWorld(sw.x, sw.y, cellSize, cellSize / 2, &swX, &swY);
                IsoToWorld(sw.floorX, sw.floorY, cellSize, cellSize / 2, &swfX, &swfY);
                Game::DrawLine(this->transform->pos.x + swX + cellSize / 2, this->transform->pos.y + swY + cellSize / 4,
                               swfX + cellSize / 2, swfY + cellSize / 4, switchLineColor);
            }
        }
    }
}
