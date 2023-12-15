#include "game.h"
#include <random>

Game::Game()
{
    wWidth = 1200;
    wHeight = 800;
    backgroundColor = {43, 39, 57, 1};
    font = LoadFontEx("font/Inter-Bold.ttf", 64, 0, 0);
    font1 = LoadFont("font/monogram.ttf");
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
    lastUpdateTime = 0;
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
    while (BlockFits(shadowBlock) && !IsBlockOutside(shadowBlock))
    {
        shadowBlock.Move(1, 0);
    }
    shadowBlock.Move(-1, 0);
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
    if (gameOver)
    {
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
    Block shadowBlock = currentBlock; // Create a copy of the current block
    if (!gameOver)
    {
        currentBlock.Move(0, -1);
        if (IsBlockOutside(currentBlock) || BlockFits(currentBlock) == false)
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
        if (IsBlockOutside(currentBlock) || BlockFits(currentBlock) == false)
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
        if (IsBlockOutside(currentBlock) || BlockFits(currentBlock) == false)
        {
            currentBlock.Move(-1, 0);
            LockBlock();
        }
    }
}

void Game::MoveBlockDownNow()
{
    while (!gameOver)
    {
        currentBlock.Move(1, 0);
        if (IsBlockOutside(currentBlock) || BlockFits(currentBlock) == false)
        {
            currentBlock.Move(-1, 0);
            LockBlock();
            break;
        }
    }
}

bool Game::IsBlockOutside(Block block)
{
    std::vector<Position> tiles = block.GetCellPositions();
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
        if (IsBlockOutside(currentBlock) || BlockFits(currentBlock) == false)
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
    if (BlockFits(currentBlock) == false)
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
    gameOver = false;
    lastUpdateTime = 0;
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

void Game::Guide(int& choice)
{
    Image image = LoadImage("img/huongdanchoi.png");
    Texture2D huongdanImage = LoadTextureFromImage(image);
    UnloadImage(image);
    ButtonO btn3("Home", 500, 200, font1);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(Color{43, 39, 57, 1});
        DrawTexture(huongdanImage, 100, 135, WHITE);
        btn3.draw();
        EndDrawing();
        
        if (btn3.update() == MOUSE_BUTTON_LEFT)
        {
            choice = 3;
            break;
        }
    }
    UnloadTexture(huongdanImage);
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

int Game::Over()
{
    ButtonO btn3("Home", 500, 200, font1);
    ButtonO btn5("PLAY AGAIN", 100, 300, font1);

    while(!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(backgroundColor);
        btn3.draw();
        btn5.draw();
        EndDrawing();
        if (btn3.update() == MOUSE_BUTTON_LEFT)
        {
            return 3;
        }
        else if (btn5.update() == MOUSE_BUTTON_LEFT)
        {
            return 0;
        }
        DrawTextEx(font, "Game Over", {200, 400}, 40, 5, PINK);
        // DrawTextEx(font, "Press R to restart", {200, 500}, 40, 5, PINK);
    }
}

void Game::GetReady()
{
    float numPosX = wWidth/2 + 20;
    float readyPosX = wWidth/2 - 70;

    while(!WindowShouldClose())
    {   
        UpdateMusicStream(music);
        BeginDrawing();
        ClearBackground(backgroundColor);
        GameInfo();
        if (isReady < 50)
            DrawTextEx(font, "Get Ready", {readyPosX, 50}, 50, 4, YELLOW);
        else if (isReady < 100)
        {
            DrawTextEx(font, "Get Ready", {readyPosX, 50}, 50, 4, backgroundColor);
            DrawTextEx(font, "3", {numPosX, 50}, 50, 4, YELLOW);
        }
        else if (isReady < 150)
        {
            DrawTextEx(font, "3", {numPosX, 50}, 50, 4, backgroundColor);
            DrawTextEx(font, "2", {numPosX, 50}, 50, 4, YELLOW);
        }
        else if (isReady < 200)
        {
            DrawTextEx(font, "2", {numPosX, 50}, 50, 4, backgroundColor);
            DrawTextEx(font, "1", {numPosX, 50}, 50, 4, YELLOW);
        }
        else if (isReady < 250)
        {
            DrawTextEx(font, "1", {numPosX, 50}, 50, 4, backgroundColor);
            DrawTextEx(font, "0", {numPosX, 50}, 50, 4, YELLOW);
        }
        else if (isReady < 300)
        {
            DrawTextEx(font, "0", {numPosX, 50}, 50, 4, backgroundColor);
        }
        EndDrawing();
        isReady++; 
        if (isReady == 300)
            break;
    }
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
    sprintf(namePlayedText, "%s", namePlayer.c_str());
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

void Game::Play(int& choice)
{   
    while (!WindowShouldClose() && choice != 3 && choice != 6)
    {
        BeginDrawing();
        ClearBackground(backgroundColor);
        UpdateMusicStream(music);

        HandleInput();
        if (EventTriggered(lastUpdateTime))
        {
            MoveBlockDown();
        }
        if (gameOver)
        {
            choice = 6;
        }
        else
        {
            GameInfo();
        }
        EndDrawing();
    }
}

void Game::EnterName(bool& mouseOnText, int& letterCount, int MAX_INPUT_CHARS)
{
    if (mouseOnText)
    {
        SetMouseCursor(MOUSE_CURSOR_IBEAM);

        int key = GetCharPressed();

        while (key > 0)
        {
            if ((key >= 32) && (key <= 125) && (letterCount < MAX_INPUT_CHARS))
            {
                namePlayer[letterCount] = (char)key;
                namePlayer[letterCount + 1] = '\0';
                letterCount++;
            }

            key = GetCharPressed();
        }

        if (IsKeyPressed(KEY_BACKSPACE))
        {
            letterCount--;
            if (letterCount < 0)
                letterCount = 0;
            namePlayer[letterCount] = '\0';
        }
    }
    else
        SetMouseCursor(MOUSE_CURSOR_DEFAULT);
}

void Game::Home(int& choice)
{
    int MAX_INPUT_CHARS = 9;
    int letterCount = 0;
    Rectangle textBox = {600, 400, 255, 50};
    bool mouseOnText = 1;
    int framesCounter = 0;

    Image image = LoadImage("img/TETRIS.png");
    Texture2D tetrisImage = LoadTextureFromImage(image);
    UnloadImage(image);

    ButtonO btn0("Let's Play", 100, 200, font1);
    ButtonO btn1("How To Play", 100, 400, font1);
    ButtonO btn2("Leader Board", 100, 600, font1);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(Color{43, 39, 57, 1});

        btn0.draw();
        btn1.draw();
        btn2.draw();

        // DrawTextEx(font1, "HOME", {839, 417}, 40, 5, PINK);
        // myButtons[0]->draw();
        // myButtons[1]->draw();
        // myButtons[2]->draw();

        if (IsMouseButtonPressed(0))
        {
            if (CheckCollisionPointRec(GetMousePosition(), textBox))
                mouseOnText = true;
            else
                mouseOnText = false;
        }
        EnterName(mouseOnText, letterCount, MAX_INPUT_CHARS);
        if (mouseOnText)
            framesCounter++;
        else
            framesCounter = 0;
        
        DrawTexture(tetrisImage, 350, 100, WHITE);
        // DrawTextEx(font, "Enter your name", {200, 400}, 40, 5, PINK);
        // DrawTextEx(font, "Press Enter to start", {200, 500}, 40, 5, PINK);
        // DrawTextEx(font, "Press H for help", {200, 600}, 40, 5, PINK);
        
        DrawRectangleRec(textBox, LIGHTGRAY);
        if (mouseOnText)
            DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, RED);
        else
            DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, DARKGRAY);

        DrawText(namePlayer.c_str(), (int)textBox.x + 5, (int)textBox.y + 8, 40, MAROON);

        if (mouseOnText)
        {
            if (letterCount < MAX_INPUT_CHARS)
            {
                if (((framesCounter / 20) % 2) == 0)
                    DrawText("_", (int)textBox.x + 8 + MeasureText(namePlayer.c_str(), 40), (int)textBox.y + 12, 40, MAROON);
            }
        }
        EndDrawing();
        if (IsKeyPressed(KEY_ENTER) || btn0.update() == MOUSE_BUTTON_LEFT)
        {
            choice = 0;
            break;
        }
        if (btn1.update() == MOUSE_BUTTON_LEFT)
        {
            choice = 1;
            break;
        }
        if (btn2.update() == MOUSE_BUTTON_LEFT)
        {
            choice = 2;
            break;
        }
    }
    UnloadTexture(tetrisImage);
}

void Game::Run()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);

    int choice = 3;
    
    while (!WindowShouldClose())
    {
        if (choice == 0)
        {
            GetReady();
            Play(choice);
        }
        else if (choice == 3)
        {
            Home(choice);
        }
        else if (choice == 1)
        {
            Guide(choice);
        }
        else if (choice == 2)
        {
            // BeginDrawing();
            // ClearBackground(PINK);
            // btn4.draw();
            // DrawTextEx(font1, "LEADERBOARD", {839, 417}, 40, 5, BLACK);
            // EndDrawing();
            // if (btn4.update() == MOUSE_BUTTON_LEFT)
            //     choice = 1;
        }
        // else if (choice == 5)
        // {
        //     BeginDrawing();
        //     ClearBackground(BLUE);
        //     btn4.draw();
        //     btn7.draw();
        //     DrawTextEx(font1, "PLAY GAME", {839, 417}, 40, 5, PINK);
        //     EndDrawing();
        //     if (btn4.update() == MOUSE_BUTTON_LEFT)
        //         choice = 1;
        //     if (btn7.update() == MOUSE_BUTTON_LEFT)
        //     {
        //         lose = 1;
        //         choice = 6;
        //     }
        // }
        else if (choice == 6)
        {
            choice = Over();
            Reset();
        }
    }
}