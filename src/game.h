#pragma once
#include "grid.h"
#include "blocks.cpp"
#include <string>
class Game
{
public:
    Game();
    ~Game();
    void Draw();
    void HandleInput();
    void MoveBlockDown();
    void MoveBlockDownNow();
    bool gameOver;
    int score;
    Music music;
    int linesCleared;
    int timePlayed;
    std::string namePlayer;

private:
    void MoveBlockLeft();
    void MoveBlockRight();
    Block GetRandomBlock();
    std::vector<Block> GetAllBlocks();
    bool IsBlockOutside();
    void RotateBlock();
    void LockBlock();
    bool BlockFits();
    bool BlockFits(Block block);
    void Reset();
    void UpdateScore(int linesCleared);
    void UpdateLinesCleared(int linesCleared);
    void WriteResultToFile();
    Grid grid;
    std::vector<Block> blocks;
    Block currentBlock;
    Block nextBlock;
    Sound rotateSound;
    Sound clearSound;
};