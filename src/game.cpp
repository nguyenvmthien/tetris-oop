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
    Image image = LoadImage("img/Guide.png");
    Image home = LoadImage("img/home.png");

    Texture2D guide = LoadTextureFromImage(image);
    Texture2D homeIcon = LoadTextureFromImage(home);

    UnloadImage(image);
    UnloadImage(home);

    ButtonO btn3(" ", 1020, 20, font1, 60, 6);   //short home

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(backgroundColor);
        btn3.draw();
        DrawTexture(homeIcon, 1065, 27, WHITE);
        DrawTextEx(font1, "HOW TO PLAY", {400, 100}, 40, 5, PINK);
        DrawTexture(guide, 100, 200, WHITE);
        EndDrawing();
        
        if (btn3.update() == MOUSE_BUTTON_LEFT)
        {
            choice = 3;
            break;
        }
    }
    UnloadTexture(guide);
    UnloadTexture(homeIcon);
}

void Game::LeaderBoard(int& choice)
{
    Image image = LoadImage("img/trophy.png");
    Image home = LoadImage("img/home.png");

    Texture2D board = LoadTextureFromImage(image);
    Texture2D homeIcon = LoadTextureFromImage(home);

    UnloadImage(image);
    UnloadImage(home);

    ButtonO btn3(" ", 1020, 20, font1, 60, 6);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(Color{43, 39, 57, 1});
        btn3.draw();
        DrawTexture(homeIcon, 1065, 27, WHITE);
        DrawTextEx(font1, "LEADERBOARD", {350, 100}, 80, 5, PINK);
        DrawTexture(board, 100, 200, WHITE);
        DrawTextEx(font1, "Name", {400, 215}, 40, 5, BLACK);
        DrawTextEx(font1, "Score", {650, 215}, 40, 5, BLACK);
        DrawTextEx(font1, "Time", {900, 215}, 40, 5, BLACK);
        EndDrawing();
        
        if (btn3.update() == MOUSE_BUTTON_LEFT)
        {
            choice = 3;
            break;
        }
    }
    UnloadTexture(board);
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

void Game::Over(int& choice)
{
    Image gameover1 = LoadImage("img/GameOverOn.png");
    Image gameover2 = LoadImage("img/GameOverOff.png");
    Image breakrecord = LoadImage("img/BreakTheRecord.png");

    Texture2D gameOverOn = LoadTextureFromImage(gameover1);
    Texture2D gameOverOff = LoadTextureFromImage(gameover2);
    Texture2D breakRecord = LoadTextureFromImage(breakrecord);

    UnloadImage(gameover1);
    UnloadImage(gameover2);
    UnloadImage(breakrecord);

    ButtonO btn3(" Play again ", 350, 400, font1, 60, 4);
    ButtonO btn5("   Home  ", 350, 600, font1, 60, 4);

    while(!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(backgroundColor);
        btn3.draw();
        btn5.draw();

        DrawTextEx(font1, "Score: ", {400, 300}, 60, 8, PINK);
        char scoreText[10];
        sprintf(scoreText, "%d", score);
        Vector2 textSize = MeasureTextEx(font, scoreText, 38, 2);
        DrawTextEx(font1, scoreText, {650, 300}, 60, 5, YELLOW);

        if (btn3.isHover() == true)
            DrawTexture(breakRecord, 0, 50, WHITE);
        else if (btn5.isHover() == true)
            DrawTexture(gameOverOn, 180, 50, WHITE);
        else
            DrawTexture(gameOverOff, 180, 50, WHITE);
        EndDrawing();

        if (btn3.update() == MOUSE_BUTTON_LEFT)
        {
            choice = 5;
            break;
        }
        else if (btn5.update() == MOUSE_BUTTON_LEFT)
        {
            choice = 3;
            break;
        }
    }
    UnloadTexture(gameOverOn);
    UnloadTexture(gameOverOff);
    UnloadTexture(breakRecord);
}

void Game::CountDown()
{
    float numPosX = wWidth/2 + 20;
    float readyPosX = wWidth/2 - 70;

    while(!WindowShouldClose() && isReady < 300)
    {   
        UpdateMusicStream(music);
        BeginDrawing();
        ClearBackground(backgroundColor);
        GameInfo();
        if (isReady < 50)
            DrawTextEx(font, "Get Ready", {readyPosX, 50}, 50, 4, YELLOW);
        else if (isReady < 100)
        {
            DrawTextEx(font, "3", {numPosX, 50}, 50, 4, YELLOW);
        }
        else if (isReady < 150)
        {
            DrawTextEx(font, "2", {numPosX, 50}, 50, 4, YELLOW);
        }
        else if (isReady < 200)
        {
            DrawTextEx(font, "1", {numPosX, 50}, 50, 4, YELLOW);
        }
        else if (isReady < 250)
        {
            DrawTextEx(font, "0", {numPosX, 50}, 50, 4, YELLOW);
        }
        EndDrawing();
        isReady++; 
    }
}

void Game::GameInfo()
{
    DrawTextEx(font, "Score", {839, 417}, 40, 5, PINK);
    DrawTextEx(font, "Next", {833, 99}, 40, 5, PINK);
    DrawTextEx(font, "Line", {370, 460}, 40, 5, PINK);
    DrawTextEx(font, "Name", {370, 99}, 40, 5, PINK);
    DrawTextEx(font, "Interval", {100, 99}, 40, 5, PINK);   

    char timeplay[10];
    sprintf(timeplay, "%d", timePlayed);
    DrawTextEx(font, timeplay, {100, 300}, 40, 5, WHITE);

    char intervalText[10];
    sprintf(intervalText, "%.1f", updateInterval());
    DrawTextEx(font, intervalText, {100, 146}, 40, 5, WHITE);

    char namePlayedText[10];
    sprintf(namePlayedText, "%s", namePlayer.c_str());
    DrawTextEx(font, namePlayedText, {400, 146}, 40, 5, WHITE);

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
    auto start = std::chrono::steady_clock::now();    
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
        auto end = std::chrono::steady_clock::now();
        timePlayed = std::chrono::duration_cast<chrono::seconds>(end - start).count();
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
    Image image1 = LoadImage("img/TETRIS1.png");
    Image image2 = LoadImage("img/TETRIS2.png");
    Image letplay = LoadImage("img/letplay.png");
    Image howtoplay = LoadImage("img/howtoplay.png");
    Image leaderboard = LoadImage("img/leaderboard.png");

    Texture2D tetrisImage1 = LoadTextureFromImage(image1);
    Texture2D tetrisImage2 = LoadTextureFromImage(image2);
    Texture2D homeImage1 = LoadTextureFromImage(letplay);
    Texture2D homeImage2 = LoadTextureFromImage(howtoplay);
    Texture2D homeImage3 = LoadTextureFromImage(leaderboard);

    UnloadImage(image1);
    UnloadImage(image2);
    UnloadImage(letplay);
    UnloadImage(howtoplay);
    UnloadImage(leaderboard);

    ButtonO btn0("        Let's Play      ", 110, 350, font1, 50, 4);
    ButtonO btn1(" How To Play     ", 500, 475, font1, 50, 4);
    ButtonO btn2("     Leader Board ", 110, 600, font1, 50, 4);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(Color{43, 39, 57, 1});

        btn0.draw();
        btn1.draw();
        btn2.draw();
        
        DrawTextEx(font1, "HOME", {500, 50}, 60, 5, PINK);
        DrawTexture(homeImage1, 200, 352, WHITE);
        DrawTexture(homeImage2, 900, 474, WHITE);
        DrawTexture(homeImage3, 150, 603, WHITE);
        if (btn0.isHover() == true || btn1.isHover() == true || btn2.isHover() == true)
                DrawTexture(tetrisImage1, 200, 100, WHITE);
            else
                DrawTexture(tetrisImage2, 200, 100, WHITE);
        
        EndDrawing();

        cout << "in home " << choice << endl;

        if (btn0.update() == MOUSE_BUTTON_LEFT)
        {
            choice = 5;
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
    UnloadTexture(tetrisImage1);
    UnloadTexture(tetrisImage2);
    UnloadTexture(homeImage1);
    UnloadTexture(homeImage2);
    UnloadTexture(homeImage3);
}

void Game::GetReady(int& choice)
{
    ButtonO btn4(" ", 1020, 20, font1, 60, 6); // HOME Ngắn
    ButtonO btn5("Start", 490, 600, font1, 60, 4);
    Rectangle textBox = {200, 400, 800, 100};

    Image home = LoadImage("img/home.png");
    Image image1 = LoadImage("img/TETRIS1.png");
    Image image2 = LoadImage("img/TETRIS2.png");

    Texture2D homeImage4 = LoadTextureFromImage(home);
    Texture2D tetrisImage1 = LoadTextureFromImage(image1);
    Texture2D tetrisImage2 = LoadTextureFromImage(image2);
    
    UnloadImage(home);
    UnloadImage(image1);
    UnloadImage(image2);

    bool mouseOnText = 1;
    int framesCounter = 0;
    int letterCount = 0;
    int MAX_INPUT_CHARS = 9;

    while(!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground({43, 39, 57, 1});
        
        DrawRectangleRec(textBox, LIGHTGRAY);
        DrawTextEx(font1, "Enter your name", {200, 350}, 30, 5, PINK);
        DrawTextEx(font1, "Maximum 9 characters", {600, 510}, 30, 5, PINK);
        DrawTextEx(font1, "GET READY", {450, 50}, 60, 5, PINK);

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
        if (mouseOnText)
            DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, RED);
        else
            DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, DARKGRAY);

        DrawText(namePlayer.c_str(), (int)textBox.x + 5, (int)textBox.y + 8 + 10, 60, MAROON);

        if (mouseOnText)
        {
            if (letterCount < MAX_INPUT_CHARS)
            {
                if (((framesCounter / 20) % 2) == 0)
                    DrawText("_", (int)textBox.x + 8 + MeasureText(namePlayer.c_str(), 60), (int)textBox.y + 15 + 10, 60, MAROON);
            }
        }

        btn4.draw();
        btn5.draw();
        DrawTexture(homeImage4, 1065, 27, WHITE);

        if (btn5.isHover() == true || btn4.isHover() == true)
            DrawTexture(tetrisImage1, 200, 100, WHITE);
        else
            DrawTexture(tetrisImage2, 200, 100, WHITE);
        EndDrawing();

        if (btn4.update() == MOUSE_BUTTON_LEFT)
        {
            choice = 3;
            break;
        }
        if (btn5.update() == MOUSE_BUTTON_LEFT || IsKeyPressed(KEY_ENTER))
        {
            choice = 0;
            break;
        }
    }
    UnloadTexture(homeImage4);
    UnloadTexture(tetrisImage1);
    UnloadTexture(tetrisImage2);
}

void Game::Run()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    int choice = 3;
    
    while (!WindowShouldClose())
    {
        if (choice == 0)
        {
            CountDown();
            Play(choice);
        }
        else if (choice == 3)
        {
            Home(choice);
            cout << "after home " << choice << endl;
        }
        else if (choice == 1)
        {
            Guide(choice);
        }
        else if (choice == 2)
        {
            LeaderBoard(choice);
        }
        else if (choice == 5)
        {
            GetReady(choice);
        }
        else if (choice == 6)
        {
            Over(choice);
            Reset();
        }
    }
}