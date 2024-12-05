#ifndef SPRITE_HPP
#define SPRITE_HPP

enum SpriteID
{
    FLOOR = 0,
    FLOOR_FINISH,
    FLOOR_START,
    FLOOR_HIGHLIGHT,
    BLOCK_UP,
    BLOCK_LONG,
    BLOCK_WIDE,

    NUM_SPRITES
};

struct Sprite
{
    SpriteID id;
    int tx = 0;
    int ty = 0;
    int tw = 0;
    int th = 0;
    int originY = 0;
    int originX = 0;
};

extern Sprite SPRITES[SpriteID::NUM_SPRITES];

#endif