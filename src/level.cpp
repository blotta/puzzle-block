#include <span>
#include <stdlib.h>

#include "level.hpp"
#include "util.hpp"

static const int cellSize = 64;

void Level::clear()
{
    for (auto& row : grid)
    {
        for (auto& cell : row)
        {
            cell = CellType::EMPTY;
        }
    }
}

void Level::set(int x, int y, CellType value)
{
    grid[y][x] = value;
}

void Level::load(const LevelData& ld)
{
    this->clear();
    this->rows = ld.rows;
    this->cols = ld.cols;
    for (int row = 0; row < ld.rows; ++row)
    {
        for (int col = 0; col < ld.cols; ++col)
        {
            int p = row * cols + col;
            char v = ld.data[p];
            switch (v)
            {
            case '0':
                this->set(col, row, CellType::EMPTY);
                break;
            case '1':
                this->set(col, row, CellType::FLOOR);
                break;
            case '2':
                this->set(col, row, CellType::START);
                break;
            case '3':
                this->set(col, row, CellType::FINISH);
                break;
            case '4':
                this->set(col, row, CellType::THIN);
                break;
            }
        }
    }

    for (int s = 0; s < ld.switchCount; s++)
    {
        this->switches[s] = ld.switches[s];
    }
    this->switchCount = ld.switchCount;
}

void Level::toLevelData(LevelData* ld)
{
    ld->cols = cols;
    ld->rows = rows;

    memset(&ld->data, 0, sizeof(ld->data));

    for (int j = 0; j < rows; j++)
    {
        for (int i = 0; i < cols; i++)
        {
            int p = j * this->cols + i;
            if (j >= this->rows || i >= this->cols)
            {
                ld->data[p] = '0';
                continue;
            }

            switch (grid[j][i])
            {
            case CellType::EMPTY:
                ld->data[p] = '0';
                break;
            case CellType::FLOOR:
                ld->data[p] = '1';
                break;
            case CellType::START:
                ld->data[p] = '2';
                break;
            case CellType::FINISH:
                ld->data[p] = '3';
                break;
            case CellType::THIN:
                ld->data[p] = '4';
                break;
            }
        }
    }

    for (int i = 0; i < this->switchCount; i++)
    {
        ld->switches[i] = this->switches[i];
    }
    ld->switchCount = this->switchCount;
}

vec2 Level::getStartPos() const
{
    for (int j = 0; j < this->rows; j++)
    {
        for (int i = 0; i < this->cols; i++)
        {
            if (grid[j][i] == CellType::START)
                return vec2(i, j);
        }
    }

    return vec2();
}

bool Level::isValidPos(const vec2& pos) const
{
    return pos.x >= 0 && pos.x < cols && pos.y >= 0 && pos.y < rows;
}

bool Level::isValid() const
{
    int startCount = 0;
    int finishCount = 0;
    bool valid = true;

    for (int j = 0; j < MAX_GRID_SIZE; j++)
    {
        for (int i = 0; i < MAX_GRID_SIZE; i++)
        {
            // if (j >= rows && grid[j][i] != CellType::EMPTY)
            //     valid = false;

            // if (i >= cols && grid[j][i] != CellType::EMPTY)
            //     valid = false;

            if (j < rows && i < cols && grid[j][i] == CellType::START)
                startCount += 1;

            if (j < rows && i < cols && grid[j][i] == CellType::FINISH)
                finishCount += 1;
        }
    }

    if (startCount != 1 || finishCount != 1)
        valid = false;

    return valid;
}

bool Level::hasFloorAt(const vec2& pos) const
{
    if (!isValidPos(pos))
        return false;

    CellType type = grid[pos.y][pos.x];

    return type == CellType::FLOOR || type == CellType::START || type == CellType::FINISH || type == CellType::THIN;
}

bool Level::hasSwitchAt(const vec2& pos, LevelSwitch** sw)
{
    for (int i = 0; i < this->switchCount; i++)
    {
        auto& _sw = this->switches[i];
        if (_sw.x == pos.x && _sw.y == pos.y)
        {
            *sw = &this->switches[i];
            return true;
        }
    }

    return false;
}

bool Level::checkAndTriggerSwitches(const vec2& pos1, const vec2& pos2)
{
    LevelSwitch* sw;
    if (this->hasSwitchAt(pos1, &sw) || this->hasSwitchAt(pos2, &sw))
    {
        if (sw->on)
            this->set(sw->floorX, sw->floorY, CellType::EMPTY);
        else
            this->set(sw->floorX, sw->floorY, CellType::THIN);
        sw->on = !sw->on;
        Game::PlaySound("assets/sfx/switch.ogg");
        return true;
    }
    return false;
}

bool Level::isCleared(const vec2& pos1, const vec2& pos2)
{
    if (this->cellAt(pos1) == CellType::FINISH && this->cellAt(pos2) == CellType::FINISH)
    {
        return true;
    }
    return false;
}

void Level::toggleThinFloor(const vec2& pos)
{
    CellType& cell = grid[pos.y][pos.x];
    switch (cell)
    {
    case CellType::EMPTY:
        cell = CellType::THIN;
        break;
    default:
        cell = CellType::EMPTY;
        break;
    }
}

CellType Level::cellAt(const vec2& pos) const
{
    return grid[pos.y][pos.x];
}

Rect Level::rect() const
{
    int minCol = MAX_GRID_SIZE, maxCol = 0, minRow = MAX_GRID_SIZE, maxRow = 0;

    for (int i = 0; i < cols; i++)
    {
        for (int j = 0; j < rows; j++)
        {
            CellType cell = grid[j][i];

            if (cell != CellType::EMPTY)
            {
                if (i < minCol)
                    minCol = i;
                if (i > maxCol)
                    maxCol = i;
                if (j < minRow)
                    minRow = j;
                if (j > maxRow)
                    maxRow = j;
            }
        }
    }

    if (minCol > maxCol || minRow > maxRow)
    {
        return {0};
    }

    int width = maxCol - minCol + 1;
    int height = maxRow - minRow + 1;

    return Rect{
        minCol,
        minRow,
        width,
        height,
    };
}

void Level::toggleFloor(const vec2& pos)
{
    CellType& cell = grid[pos.y][pos.x];
    switch (cell)
    {
    case CellType::EMPTY:
        cell = CellType::FLOOR;
        break;
    case CellType::FLOOR:
    case CellType::START:
    case CellType::FINISH:
    case CellType::THIN:
        cell = CellType::EMPTY;
        break;
    default:
        break;
    }
}

void Level::toggleSpecialFloor(const vec2& pos)
{
    CellType& cell = grid[pos.y][pos.x];
    switch (cell)
    {
    case CellType::START:
        cell = CellType::FINISH;
        break;
    case CellType::FINISH:
        cell = CellType::THIN;
        break;
    case CellType::THIN:
        cell = CellType::EMPTY;
        break;
    default:
        cell = CellType::START;
        break;
    }
}

void Level::addRow()
{
    if (rows < MAX_GRID_SIZE)
    {
        rows++;
        for (int i = 0; i < cols; i++)
            grid[rows - 1][i] = CellType::EMPTY;
    }
}

void Level::removeRow()
{
    if (rows > 1)
        rows--;
}

void Level::addColumn()
{
    if (cols < MAX_GRID_SIZE)
    {
        cols++;
        for (int i = 0; i < rows; i++)
            grid[i][cols - 1] = CellType::EMPTY;
    }
}

void Level::removeColumn()
{
    if (cols > 1)
        cols--;
}

void Level::addSwitch(LevelSwitch sw)
{
    if (this->switchCount == MAX_SWITCH_COUNT)
        return;
    this->switches[this->switchCount] = sw;
    this->switchCount++;
}

void Level::removeSwitch(const vec2& pos)
{
    bool removed = false;
    for (int i = 0; i < this->switchCount; i++)
    {
        if (removed)
        {
            this->switches[i - 1] = this->switches[i];
            this->switches[i] = {};
            continue;
        }

        if (this->switches[i].x == pos.x && this->switches[i].y == pos.y)
        {
            removed = true;
        }
    }

    if (removed)
    {
        this->switchCount--;
        for (int j = 0; j < MAX_GRID_SIZE; j++)
        {
            for (int i = 0; i < MAX_GRID_SIZE; i++)
            {
                if (grid[j][i] == CellType::THIN)
                    grid[j][i] = CellType::EMPTY;
            }
        }
    }
}

void Level::trim()
{
    auto rect = this->rect();

    if (rect.x > 0 || rect.y > 0)
    {
        this->moveCellsBy({-rect.x, -rect.y});
    }

    this->cols = rect.w;
    this->rows = rect.h;
}

void Level::moveCellsBy(const vec2& diff)
{
    bool canMakeMove = true;

    auto rect = this->rect();

    int emptyLeft = rect.x;
    int emptyRight = cols - rect.w - rect.x;
    int emptyTop = rect.y;
    int emptyBottom = rows - rect.h - rect.y;

    if (diff.x < 0 && emptyLeft < -diff.x)
    {
        Log::error("Cannot move cells by %d to the left", -diff.x);
        canMakeMove = false;
    }

    if (diff.x > 0 && emptyRight < diff.x)
    {
        Log::error("Cannot move cells by %d to the right", diff.x);
        canMakeMove = false;
    }

    if (diff.y < 0 && emptyTop < -diff.y)
    {
        Log::error("Cannot move cells by %d to the top", -diff.y);
        canMakeMove = false;
    }

    if (diff.y > 0 && emptyBottom < diff.y)
    {
        Log::error("Cannot move cells by %d to the bottom", diff.y);
        canMakeMove = false;
    }

    if (!canMakeMove)
        return;

    std::array<std::array<CellType, MAX_GRID_SIZE>, MAX_GRID_SIZE> oldGrid = this->grid;

    for (int i = 0; i < MAX_GRID_SIZE; i++)
    {
        for (int j = 0; j < MAX_GRID_SIZE; j++)
        {
            int oldI = i - diff.x;
            int oldJ = j - diff.y;
            if (i < this->cols && j < this->rows && oldI >= 0 && oldI < MAX_GRID_SIZE && oldJ >= 0 && oldJ < MAX_GRID_SIZE)
            {
                this->grid[j][i] = oldGrid[oldJ][oldI];
            }
            else
                this->grid[j][i] = CellType::EMPTY;
        }
    }

    for (int i = 0; i < this->switchCount; i++)
    {
        auto& sw = this->switches[i];
        sw.x += diff.x;
        sw.y += diff.y;
        sw.floorX += diff.x;
        sw.floorY += diff.y;
    }
}

void Level::centerCells()
{
    auto rect = this->rect();

    int emptyLeft = rect.x;
    int emptyRight = cols - rect.w - rect.x;
    int emptyTop = rect.y;
    int emptyBottom = rows - rect.h - rect.y;

    vec2 moveBy;

    int marginX = (int)SDL_floorf((emptyLeft + emptyRight) / 2.f);
    int marginY = (int)SDL_floorf((emptyTop + emptyBottom) / 2.f);

    this->moveCellsBy({marginX - emptyLeft, marginY - emptyTop});
}

void LevelVisual::startRise(std::function<void()> onComplete)
{
    mVisualState = LevelVisualState::RISING;
    animRise.duration = 2.f;
    animRise.start();
    animRise.events.clear();
    animRise.onComplete = onComplete;

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

void LevelVisual::init(const LevelData& ld)
{
    mModel.load(ld);
    mVisualState = LevelVisualState::IDLE;
    animRise.stop();
}

void LevelVisual::update(float dt)
{
    if (mVisualState == LevelVisualState::IDLE)
    {
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

void LevelVisual::draw()
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

            Game::DrawSprite(this->pos.x + sx + posDiff.x, this->pos.y + sy + posDiff.y, sprId);

            for (int sidx = 0; sidx < this->mModel.switchCount; sidx++)
            {
                auto& sw = this->mModel.switches[sidx];
                if (sw.x != i || sw.y != j)
                    continue;

                int sx;
                int sy;
                IsoToWorld(sw.x, sw.y, cellSize, cellSize / 2, &sx, &sy);

                Game::DrawSprite(this->pos.x + sx + posDiff.x, this->pos.y + sy + posDiff.y,
                                 sw.on ? SpriteID::SPR_SWITCH_ON : SpriteID::SPR_SWITCH_OFF);
            }
        }
    }
}