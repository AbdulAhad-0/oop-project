#include "game.h"
#include <random>

Game::Game()
{
    grid = Grid();
    blocks = GetAllBlocks();
    crrblock = GetRandomBlock();
    nxtblck = GetRandomBlock();
    gmeover = false;
    score = 0;
    InitAudioDevice();
    music = LoadMusicStream("Sounds/music.mp3");
    PlayMusicStream(music);
    rotsound = LoadSound("Sounds/rotate.mp3");
    clrsound = LoadSound("Sounds/clear.mp3");
}

Game::~Game()
{
    UnloadSound(rotsound);
    UnloadSound(clrsound);
    UnloadMusicStream(music);
    CloseAudioDevice();
}

Block Game::GetRandomBlock()
{
    if (blocks.empty())
    {
        blocks = GetAllBlocks();
    }
    int randonin = rand() % blocks.size();
    Block block = blocks[randonin];
    blocks.erase(blocks.begin() + randonin);
    return block;
}

std::vector<Block> Game::GetAllBlocks()
{
    return {IBlock(), JBlock(), LBlock(), OBlock(), SBlock(), TBlock(), ZBlock()};
}

void Game::Draw()
{
    grid.Draw();
    crrblock.Draw(11, 11);
    switch (nxtblck.id)
    {
    case 3:
        nxtblck.Draw(255, 290);
        break;
    case 4:
        nxtblck.Draw(255, 280);
        break;
    default:
        nxtblck.Draw(270, 270);
        break;
    }
}

void Game::HandleInput()
{
    int keypress = GetKeyPressed();
    if (gmeover && keypress != 0)
    {
        gmeover = false;
        rest();
    }
    switch (keypress)
    {
    case KEY_LEFT:
        moveblkleft();
        break;
    case KEY_RIGHT:
        moveblkrgt();
        break;
    case KEY_DOWN:
        moveblkdwn();
        uptscore(0, 1);
        break;
    case KEY_UP:
        rotateblk();
        break;
    }
}

void Game::moveblkleft()
{
    if (!gmeover)
    {
        crrblock.Move(0, -1);
        if (isblkoutsde() || blkfits() == false)
        {
            crrblock.Move(0, 1);
        }
    }
}

void Game::moveblkrgt()
{
    if (!gmeover)
    {
        crrblock.Move(0, 1);
        if (isblkoutsde() || blkfits() == false)
        {
            crrblock.Move(0, -1);
        }
    }
}

void Game::moveblkdwn()
{
    if (!gmeover)
    {
        crrblock.Move(1, 0);
        if (isblkoutsde() || blkfits() == false)
        {
            crrblock.Move(-1, 0);
            lckblk();
        }
    }
}

bool Game::isblkoutsde()
{
    std::vector<Position> tiles = crrblock.GetCellPositions();
    for (Position item : tiles)
    {
        if (grid.IsCellOutside(item.row, item.column))
        {
            return true;
        }
    }
    return false;
}

void Game::rotateblk()
{
    if (!gmeover)
    {
        crrblock.Rotate();
        if (isblkoutsde() || blkfits() == false)
        {
            crrblock.UndoRotation();
        }
        else
        {
            PlaySound(rotsound);
        }
    }
}

void Game::lckblk()
{
    std::vector<Position> tiles = crrblock.GetCellPositions();
    for (Position item : tiles)
    {
        grid.grid[item.row][item.column] = crrblock.id;
    }
    crrblock = nxtblck;
    if (blkfits() == false)
    {
        gmeover = true;
    }
    nxtblck = GetRandomBlock();
    int rowsCleared = grid.ClearFullRows();
    if (rowsCleared > 0)
    {
        PlaySound(clrsound);
        uptscore(rowsCleared, 0);
    }
}

bool Game::blkfits()
{
    std::vector<Position> tiles = crrblock.GetCellPositions();
    for (Position item : tiles)
    {
        if (grid.IsCellEmpty(item.row, item.column) == false)
        {
            return false;
        }
    }
    return true;
}

void Game::rest()
{
    grid.Initialize();
    blocks = GetAllBlocks();
    crrblock = GetRandomBlock();
    nxtblck = GetRandomBlock();
    score = 0;
}

void Game::uptscore(int lineclr, int movedwnpts)
{
    switch (lineclr)
    {
    case 1:
        score += 100;
        break;
    case 2:
        score += 300;
        break;
    case 3:
        score += 500;
        break;
    default:
        break;
    }

    score += movedwnpts;
}
