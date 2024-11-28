#ifndef BLOCK_HPP
#define BLOCK_HPP

enum BlockState
{
    UP,
    WIDE,
    LONG
};
struct Block
{
    int x;
    int y;
    BlockState state = BlockState::UP; 
};

#endif