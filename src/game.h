#pragma once
#include "grid.h"
#include "blocks.cpp"
#include <string>
#include <iostream>
#include "myGui.hpp"
#include <chrono>

struct Player {
    char name[10];
    int score;
    int timePlayed;
};
class Game
{
public:
    Game();
    ~Game();
    void LoadGameTexture();
    void UnloadGameTexture();
    void Draw();
    void HandleInput();
    void MoveBlockDown();
    void MoveBlockDownNow();
    void Guide(int& choice);
    double updateInterval();
    bool EventTriggered(double& lastUpdateTime);
    void Over(int& choice);
    void CountDown();
    void GameInfo();
    void Play(int& choice);
    void EnterName(bool& mouseOnText, int& letterCount, int MAX_INPUT_CHARS);
    void Home(int& choice);
    void Run();
    void LeaderBoard(int& choice);
    void GetReady(int& choice);
    void InfoGroup(int& choice);
    int wWidth;
    int wHeight;
    Color backgroundColor;
    Font font;
    Font font1;
    bool gameOver;
    int score;
    int isReady;
    Music music;
    bool isMute;
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
    Texture2D tetrisImage1;
    Texture2D tetrisImage2;
    Texture2D tetrisImage3;
    Texture2D homeImage1;
    Texture2D homeImage2;
    Texture2D homeImage3;
    Texture2D homeImage4;
    Texture2D trophyImage;
    Texture2D gameOverOn;
    Texture2D gameOverOff;
    Texture2D breakRecord;
    Texture2D soundOn;
    Texture2D soundOff;
    Texture2D aboutUs;
    Texture2D leaderBoard;
    Texture2D guide;
    Texture2D logo;
    Texture2D aboutUsNormal;
    Texture2D aboutUsHover;
    std::vector<Player> topPlayers;
    void getLeaderBoard();
};