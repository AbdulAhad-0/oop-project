#pragma once
#include "grid.h"
#include "blocks.cpp"

class Game
{
public:
    Game();
    ~Game();
    void Draw();
    void HandleInput();
    void moveblkdwn();
    bool gmeover;
    int score;
    Music music;

private:
    void moveblkleft();
    void moveblkrgt();
    Block GetRandomBlock();
    std::vector<Block> GetAllBlocks();
    bool isblkoutsde();
    void rotateblk();
    void lckblk();
    bool blkfits();
    void rest();
    void uptscore(int lineclr, int movedwnpts);
    Grid grid;
    std::vector<Block> blocks;
    Block crrblock;
    Block nxtblck;
    Sound rotsound;
    Sound clrsound;
};