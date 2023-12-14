#include "game.h"
#include <random>

Game::Game()
{
    wWidth = 1200;
    wHeight = 800;
    playBackColor = {43, 39, 57, 1};
    font = LoadFontEx("Font/Inter-Bold.ttf", 64, 0, 0);
    grid = Grid();
    blocks = GetAllBlocks();
    currentBlock = GetRandomBlock();
    nextBlock = GetRandomBlock();
    gameOver = false;
    score = 0;
    linesCleared = 0;
    timePlayed = 0;
    isReady = 0;
    namePlayer = "";
    InitAudioDevice();
    music = LoadMusicStream("sounds/music.mp3");
    PlayMusicStream(music);
    rotateSound = LoadSound("sounds/rotate.mp3");
    clearSound = LoadSound("sounds/clear.mp3");
}

Game::~Game()
{
    UnloadSound(rotateSound);
    UnloadSound(clearSound);
    UnloadMusicStream(music);
    CloseAudioDevice();
}

Block Game::GetRandomBlock()
{
    if (blocks.empty())
    {
        blocks = GetAllBlocks();
    }
    int randomIndex = rand() % blocks.size();
    Block block = blocks[randomIndex];
    blocks.erase(blocks.begin() + randomIndex);
    return block;
}

std::vector<Block> Game::GetAllBlocks()
{
    return {IBlock(), JBlock(), LBlock(), OBlock(), SBlock(), TBlock(), ZBlock()};
}

void Game::Draw()
{
    grid.Draw();
    // Draw the shadow of the current block
    Block shadowBlock = currentBlock; // Create a copy of the current block
    while (BlockFits(shadowBlock))
    {
        shadowBlock.Move(1, 0); // Move the shadow block down
    }
    shadowBlock.Move(-1, 0); // Adjust the position

    // Draw the shadow block with a semi-transparent color
    shadowBlock.Draw(489, 146, Fade(GRAY, 0.5f));

    // Draw the actual current block
    currentBlock.Draw(489, 146);

    switch (nextBlock.id)
    {
    case 3:
        nextBlock.Draw(838, 231);
        break;
    case 4:
        nextBlock.Draw(838, 221);
        break;
    default:
        nextBlock.Draw(853, 211);
        break;
    }
}

void Game::HandleInput()
{
    int keyPressed = GetKeyPressed();
    if (gameOver && keyPressed == KEY_R)
    {
        gameOver = false;
        Reset();
    }
    switch (keyPressed)
    {
    case KEY_LEFT:
        MoveBlockLeft();
        break;
    case KEY_RIGHT:
        MoveBlockRight();
        break;
    case KEY_DOWN:
        MoveBlockDown();
        break;
    case KEY_UP:
        RotateBlock();
        break;
    case KEY_SPACE:
        MoveBlockDownNow();
        break;
    }
}

void Game::MoveBlockLeft()
{
    if (!gameOver)
    {
        currentBlock.Move(0, -1);
        if (IsBlockOutside() || BlockFits() == false)
        {
            currentBlock.Move(0, 1);
        }
    }
}

void Game::MoveBlockRight()
{
    if (!gameOver)
    {
        currentBlock.Move(0, 1);
        if (IsBlockOutside() || BlockFits() == false)
        {
            currentBlock.Move(0, -1);
        }
    }
}

void Game::MoveBlockDown()
{
    if (!gameOver)
    {
        currentBlock.Move(1, 0);
        if (IsBlockOutside() || BlockFits() == false)
        {
            currentBlock.Move(-1, 0);
            LockBlock();
        }
    }
}

void Game::MoveBlockDownNow() 
{
    while(!gameOver) 
    {
        currentBlock.Move(1, 0);
        if (IsBlockOutside() || BlockFits() == false)
        {
            currentBlock.Move(-1, 0);
            LockBlock();
            break;
        }
    }
}

bool Game::IsBlockOutside()
{
    std::vector<Position> tiles = currentBlock.GetCellPositions();
    for (Position item : tiles)
    {
        if (grid.IsCellOutside(item.row, item.column))
        {
            return true;
        }
    }
    return false;
}

void Game::RotateBlock()
{
    if (!gameOver)
    {
        currentBlock.Rotate();
        if (IsBlockOutside() || BlockFits() == false)
        {
            currentBlock.UndoRotation();
        }
        else
        {
            PlaySound(rotateSound);
        }
    }
}

void Game::LockBlock()
{
    std::vector<Position> tiles = currentBlock.GetCellPositions();
    for (Position item : tiles)
    {
        grid.grid[item.row][item.column] = currentBlock.id;
    }
    currentBlock = nextBlock;
    if (BlockFits() == false)
    {
        gameOver = true;
        timePlayed = GetTime();
        WriteResultToFile();
    }
    nextBlock = GetRandomBlock();
    int rowsCleared = grid.ClearFullRows();
    if (rowsCleared > 0)
    {
        PlaySound(clearSound);
        UpdateScore(rowsCleared);
        UpdateLinesCleared(rowsCleared);
    }
}

bool Game::BlockFits()
{
    std::vector<Position> tiles = currentBlock.GetCellPositions();
    for (Position item : tiles)
    {
        if (grid.IsCellEmpty(item.row, item.column) == false)
        {
            return false;
        }
    }
    return true;
}

bool Game::BlockFits(Block block)
{
    std::vector<Position> tiles = block.GetCellPositions();
    for (Position item : tiles)
    {
        if (grid.IsCellEmpty(item.row, item.column) == false)
        {
            return false;
        }
    }
    return true;
}

void Game::Reset()
{
    grid.Initialize();
    blocks = GetAllBlocks();
    currentBlock = GetRandomBlock();
    nextBlock = GetRandomBlock();
    score = 0;
    linesCleared = 0;
    timePlayed = 0;
    isReady = 0;
}

void Game::UpdateScore(int linesCleared)
{
    score += linesCleared * 100;
}

void Game::UpdateLinesCleared(int linesCleared)
{
    this->linesCleared += linesCleared;
}

void Game::WriteResultToFile()
{
    // if (namePlayer != "")
    // {
        FILE *file = fopen("scores.txt", "a");
        if (file == NULL)
        {
            return;
        }
        fprintf(file, "%s-", namePlayer.c_str());
        fprintf(file, "%d-", score);
        fprintf(file, "%d\n", timePlayed);
        fclose(file);
    // }
}

void Game::HuongDan()
{
    while (!IsKeyPressed(KEY_ENTER) )
    {
        Image image = LoadImage("img/huongdanchoi.png");
        Texture2D huongdanImage = LoadTextureFromImage(image);
        UnloadImage(image);
        BeginDrawing();
        ClearBackground(Color{43, 39, 57, 1});
        DrawTexture(huongdanImage, 100, 135, WHITE);
        EndDrawing();
        UnloadTexture(huongdanImage);
    }
}

double Game::updateInterval()
{
    double interval = 0.5;
    interval = interval - (linesCleared / 5) * 0.1 > 0.2 ? interval - (linesCleared / 5) * 0.1 : 0.2;
    return interval;
}

bool Game::EventTriggered(double& lastUpdateTime)
{   
    double currentTime = GetTime();
    if (currentTime - lastUpdateTime >= updateInterval())
    {
        lastUpdateTime = currentTime;
        return true;
    }
    return false;
}

void Game::Over(double& lastUpdateTime)
{
    DrawTextEx(font, "Game Over", {200, 400}, 40, 5, PINK);
    DrawTextEx(font, "Press R to restart", {200, 500}, 40, 5, PINK);
    lastUpdateTime = 0;
}

void Game::GetReady(float& numPosX, float& readyPosX)
{
    if (isReady < 50)
    {
        DrawTextEx(font, "Get Ready", {readyPosX, 50}, 50, 4, YELLOW);
    }
    else if (isReady < 100)
    {
        DrawTextEx(font, "Get Ready", {readyPosX, 50}, 50, 4, playBackColor);
        DrawTextEx(font, "3", {numPosX, 50}, 50, 4, YELLOW);
    }
    else if (isReady < 150)
    {
        DrawTextEx(font, "3", {numPosX, 50}, 50, 4, playBackColor);
        DrawTextEx(font, "2", {numPosX, 50}, 50, 4, YELLOW);
    }
    else if (isReady < 200)
    {
        DrawTextEx(font, "2", {numPosX, 50}, 50, 4, playBackColor);
        DrawTextEx(font, "1", {numPosX, 50}, 50, 4, YELLOW);
    }
    else if (isReady < 250)
    {
        DrawTextEx(font, "1", {numPosX, 50}, 50, 4, playBackColor);
        DrawTextEx(font, "0", {numPosX, 50}, 50, 4, YELLOW);
    }
    else
    {
        DrawTextEx(font, "0", {numPosX, 50}, 50, 4, playBackColor);
    }
    isReady++;
}

void Game::GameInfo()
{
    DrawTextEx(font, "Score", {839, 417}, 40, 5, PINK);
    DrawTextEx(font, "Next", {833, 99}, 40, 5, PINK);
    DrawTextEx(font, "Line", {370, 460}, 40, 5, PINK);
    DrawTextEx(font, "Name", {370, 99}, 40, 5, PINK);
    DrawTextEx(font, "Interval", {100, 99}, 40, 5, PINK);   

    char intervalText[10];
    sprintf(intervalText, "%.1f", updateInterval());
    DrawTextEx(font, intervalText, {100, 146}, 40, 5, WHITE);

    char namePlayedText[10];
    sprintf(namePlayedText, "%d", namePlayer.c_str());
    DrawTextEx(font, namePlayedText, {400, 146}, 40, 5, WHITE);

    // DrawRectangleRounded({863, 482, 250, 100}, 1, 6, lightBlue);
    char scoreText[10];
    sprintf(scoreText, "%d", score);
    Vector2 textSize = MeasureTextEx(font, scoreText, 38, 2);

    DrawTextEx(font, scoreText, {839 + 55 - textSize.x/2, 460}, 40, 5, WHITE);

    char linesClearedText[10];
    sprintf(linesClearedText, "%d", linesCleared);
    DrawTextEx(font, linesClearedText, {400, 511}, 40, 5, WHITE);

    DrawRectangleRounded({863, 146, 250, 200}, 0.5, 6, darkGrey);
    Draw();
}

void Game::Play(double& lastUpdateTime)
{   
    float numPosX = wWidth/2 + 20;
    float readyPosX = wHeight/2 - 70;
    while (WindowShouldClose() == false)
    {
        BeginDrawing();
        ClearBackground(playBackColor);
        UpdateMusicStream(music);

        if (isReady < 300)
            GetReady(numPosX, readyPosX);
        else 
        {
            HandleInput();
            if (EventTriggered(lastUpdateTime))
            {
                MoveBlockDown();
            }
        }
        if (gameOver)
        {
            Over(lastUpdateTime);
        }
        else
        {
            GameInfo();
        }
        EndDrawing();
    }
}
