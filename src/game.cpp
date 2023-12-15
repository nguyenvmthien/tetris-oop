#include "game.h"
#include <random>
#include <algorithm>

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
    logo = LoadTextureFromImage(LoadImage("img/TETRISsmall.png"));
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
    UnloadTexture(logo);
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

void Game::Guide(int &choice)
{
    Image image = LoadImage("img/huongdanchoi.png");
    Texture2D huongdanImage = LoadTextureFromImage(image);
    UnloadImage(image);
<<<<<<< Updated upstream
    ButtonO btn3("Home", 500, 200, font1);

    == == == =
                 UnloadImage(home);

    ButtonO btn3(" ", 1020, 20, font1, 60, 6); // short home

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

void Game::LeaderBoard(int &choice)
{
    Image image = LoadImage("img/trophy.png");
    Image home = LoadImage("img/home.png");

    Texture2D board = LoadTextureFromImage(image);
    Texture2D homeIcon = LoadTextureFromImage(home);

    UnloadImage(image);
    UnloadImage(home);

    ButtonO btn3(" ", 1020, 20, font1, 60, 6);
    getLeaderBoard();
>>>>>>> Stashed changes
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(Color{43, 39, 57, 1});
        DrawTexture(huongdanImage, 100, 135, WHITE);
        btn3.draw();
<<<<<<< Updated upstream
        == == == =
                     DrawTexture(homeIcon, 1065, 27, WHITE);
        DrawTextEx(font1, "LEADERBOARD", {350, 100}, 80, 5, PINK);
        DrawTexture(board, 100, 200, WHITE);
        DrawTextEx(font1, "Name", {400, 215}, 40, 5, BLACK);
        DrawTextEx(font1, "Score", {650, 215}, 40, 5, BLACK);
        DrawTextEx(font1, "Time", {900, 215}, 40, 5, BLACK);
        // 1st
        char nameText[10];
        sprintf(nameText, "%s", topPlayers[1].name);
        DrawTextEx(font1, nameText, {350, 300}, 50, 5, red);
        char scoreText[10];
        sprintf(scoreText, "%d", topPlayers[1].score);
        DrawTextEx(font1, scoreText, {650, 300}, 60, 5, red);
        char timePlayedText[10];
        sprintf(timePlayedText, "%d", topPlayers[1].timePlayed);
        DrawTextEx(font1, timePlayedText, {900, 300}, 60, 5, red);

        // 2nd
        sprintf(nameText, "%s", topPlayers[2].name);
        DrawTextEx(font1, nameText, {350, 450}, 50, 5, {26, 31, 40, 255});
        sprintf(scoreText, "%d", topPlayers[2].score);
        DrawTextEx(font1, scoreText, {650, 450}, 60, 5, {26, 31, 40, 255});
        sprintf(timePlayedText, "%d", topPlayers[2].timePlayed);
        DrawTextEx(font1, timePlayedText, {900, 450}, 60, 5, {26, 31, 40, 255});

        // 3rd
        sprintf(nameText, "%s", topPlayers[3].name);
        DrawTextEx(font1, nameText, {350, 600}, 50, 5, {255, 107, 1, 255});
        sprintf(scoreText, "%d", topPlayers[3].score);
        DrawTextEx(font1, scoreText, {650, 600}, 60, 5, {255, 107, 1, 255});
        sprintf(timePlayedText, "%d", topPlayers[3].timePlayed);
        DrawTextEx(font1, timePlayedText, {900, 600}, 60, 5, {255, 107, 1, 255});
>>>>>>> Stashed changes
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

bool Game::EventTriggered(double &lastUpdateTime)
{
    double currentTime = GetTime();
    if (currentTime - lastUpdateTime >= updateInterval())
    {
        lastUpdateTime = currentTime;
        return true;
    }
    return false;
}

<<<<<<< Updated upstream
int Game::Over() == == == =
                              void Game::Over(int &choice)
>>>>>>> Stashed changes
{
    ButtonO btn3("Home", 500, 200, font1);
    ButtonO btn5("PLAY AGAIN", 100, 300, font1);

    while (!WindowShouldClose())
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
    float numPosX = wWidth / 2 + 20;
    float numPosY = wHeight / 2 + 20;
    float readyPosY = wHeight / 2 - 70;

    while (!WindowShouldClose() && isReady < 300)
    {
        UpdateMusicStream(music);
        BeginDrawing();
        ClearBackground(backgroundColor);
        GameInfo();
        if (isReady < 50)
            DrawTextEx(font, "Get Ready", {400, readyPosY}, 100, 4, YELLOW);
        else if (isReady < 100)
        {
            DrawTextEx(font, "3", {numPosX, readyPosY}, 100, 4, YELLOW);
        }
        else if (isReady < 150)
        {
            DrawTextEx(font, "2", {numPosX, readyPosY}, 100, 4, YELLOW);
        }
        else if (isReady < 200)
        {
            DrawTextEx(font, "1", {numPosX, readyPosY}, 100, 4, YELLOW);
        }
        else if (isReady < 250)
        {
            DrawTextEx(font, "0", {numPosX, readyPosY}, 100, 4, YELLOW);
        }

        EndDrawing();
        isReady++;
    }
}

void Game::GameInfo()
{
    DrawTextEx(font, "Score", {100, 440}, 40, 5, PINK);
    DrawTextEx(font, "Next", {850, 99}, 40, 5, PINK);
    DrawTextEx(font, "Line", {900, 440}, 40, 5, PINK);
    DrawTextEx(font, "Player", {100, 200}, 40, 5, PINK);
    DrawTextEx(font, "Time", {900, 600}, 40, 5, PINK);
    DrawTexture(logo, 100, 40, WHITE);
    // DrawTextEx(font, "Interval", {100, 99}, 40, 5, PINK);

    // char intervalText[10];
    // sprintf(intervalText, "%.1f", updateInterval());
    // DrawTextEx(font, intervalText, {100, 146}, 40, 5, WHITE);
    char timePlayedText[10];
    sprintf(timePlayedText, "%d", timePlayed);
    DrawTextEx(font, timePlayedText, {900, 660}, 70, 5, WHITE);

    char namePlayedText[10];
    sprintf(namePlayedText, "%s", namePlayer.c_str());
    DrawTextEx(font, namePlayedText, {100, 260}, 70, 5, WHITE);

    // DrawRectangleRounded({863, 482, 250, 100}, 1, 6, lightBlue);
    char scoreText[10];
    sprintf(scoreText, "%d", score);
    Vector2 textSize = MeasureTextEx(font, scoreText, 38, 2);
    DrawTextEx(font, scoreText, {100, 500}, 70, 5, WHITE);

    char linesClearedText[10];
    sprintf(linesClearedText, "%d", linesCleared);
    DrawTextEx(font, linesClearedText, {900, 500}, 70, 5, WHITE);

    DrawRectangleRounded({863, 146, 250, 200}, 0.5, 6, darkGrey);
    Draw();
}

void Game::Play(int &choice)
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

void Game::EnterName(bool &mouseOnText, int &letterCount, int MAX_INPUT_CHARS)
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

void Game::Home(int &choice)
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
<<<<<<< Updated upstream

        // DrawTextEx(font1, "HOME", {839, 417}, 40, 5, PINK);
        // myButtons[0]->draw();
        // myButtons[1]->draw();
        // myButtons[2]->draw();
        == == == =

                     DrawTextEx(font1, "HOME", {500, 50}, 60, 5, PINK);
        DrawTexture(homeImage1, 200, 352, WHITE);
        DrawTexture(homeImage2, 900, 474, WHITE);
        DrawTexture(homeImage3, 150, 603, WHITE);
        if (btn0.isHover() == true || btn1.isHover() == true || btn2.isHover() == true)
            DrawTexture(tetrisImage1, 200, 100, WHITE);
        else
            DrawTexture(tetrisImage2, 200, 100, WHITE);

        EndDrawing();

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

void Game::GetReady(int &choice)
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

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground({43, 39, 57, 1});

        DrawRectangleRec(textBox, LIGHTGRAY);
        DrawTextEx(font1, "Enter your name", {200, 350}, 30, 5, PINK);
        DrawTextEx(font1, "Maximum 9 characters", {600, 510}, 30, 5, PINK);
        DrawTextEx(font1, "GET READY", {450, 50}, 60, 5, PINK);
>>>>>>> Stashed changes

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

void Game::getLeaderBoard()
{
    topPlayers.clear();
    FILE *file = fopen("scores.txt", "r");
    if (file == NULL)
    {
        std::cout << "cannot open score" << std::endl;
        return;
    }
    char name[10];
    int score;
    int timePlayed;
    int i = 1;
    while (fscanf(file, "%[^-]-%d-%d\n", name, &score, &timePlayed) != EOF)
    {
        std::cout << i++ << std::endl;
        Player player;
        strcpy(player.name, name);
        player.score = score;
        player.timePlayed = timePlayed;
        topPlayers.push_back(player);
    }
    fclose(file);

    std::sort(topPlayers.begin(), topPlayers.end(), [](Player a, Player b)
              { return a.score > b.score; });

    FILE *file1 = fopen("topPlayer.txt", "w");
    if (file1 == NULL)
    {
        std::cout << "cannot open txt" << std::endl;
        return;
    }
    for (const auto &i : topPlayers)
    {
        std::cout << "write" << std::endl;
        fprintf(file1, "%s-%d-%d\n", i.name, i.score, i.timePlayed);
    }

    fclose(file1);
}