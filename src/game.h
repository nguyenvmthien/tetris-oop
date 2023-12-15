#pragma once
#include "grid.h"
#include "blocks.cpp"
#include <string>
#include <iostream>
#include "myGui.hpp"
class Game
{
public:
    Game();
    ~Game();
    void Draw();
    void HandleInput();
    void MoveBlockDown();
    void MoveBlockDownNow();
    void Guide(int& choice);
    double updateInterval();
    bool EventTriggered(double& lastUpdateTime);
    int Over();
    void GetReady();
    void GameInfo();
    void Play(int& choice);
    void EnterName(bool& mouseOnText, int& letterCount, int MAX_INPUT_CHARS);
    void Home(int& choice);
    void Run();
    int wWidth;
    int wHeight;
    Color backgroundColor;
    Font font;
    Font font1;
    bool gameOver;
    int score;
    int isReady;
    Music music;
    int linesCleared;
    int timePlayed;
    std::string namePlayer;
    double lastUpdateTime;

private:
    void MoveBlockLeft();
    void MoveBlockRight();
    Block GetRandomBlock();
    std::vector<Block> GetAllBlocks();
    bool IsBlockOutside(Block block);
    void RotateBlock();
    void LockBlock();
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