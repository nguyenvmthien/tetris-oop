#pragma once
#include "grid.h"
#include "blocks.cpp"
#include <string>
#include <iostream>
class Game
{
public:
    Game();
    ~Game();
    void Draw();
    void HandleInput();
    void MoveBlockDown();
    void MoveBlockDownNow();
    void Guide();
    double updateInterval();
    bool EventTriggered(double& lastUpdateTime);
    void Over(double& lastUpdateTim);
    void GetReady();
    void GameInfo();
    void Play(double& lastUpdateTime);
    void EnterName(bool& mouseOnText, int& letterCount, int MAX_INPUT_CHARS);
    void Home();
    int wWidth;
    int wHeight;
    Color backgroundColor;
    Font font;
    bool gameOver;
    int score;
    int isReady;
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
