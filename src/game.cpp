#include "game.h"
#include <random>
#include <algorithm>
#include <string.h>
Game::Game()
{
    wWidth = 1200;
    wHeight = 800;
    backgroundColor = {43, 39, 57, 1};
    font = LoadFontEx("Font/Inter-Bold.ttf", 64, 0, 0);
    font1 = LoadFont("Font/monogram.ttf");
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
    isMute = 0;
    InitAudioDevice();
    music = LoadMusicStream("Sounds/music.mp3");
    PlayMusicStream(music);
    rotateSound = LoadSound("Sounds/rotate.mp3");
    clearSound = LoadSound("Sounds/clear.mp3");
    LoadGameTexture();
}

Game::~Game()
{
    UnloadGameTexture();
    UnloadSound(rotateSound);
    UnloadSound(clearSound);
    UnloadMusicStream(music);
    CloseAudioDevice();
}

void Game::LoadGameTexture()
{
    tetrisImage1 = LoadTextureFromImage(LoadImage("img/TETRIS1.png"));
    tetrisImage2 = LoadTextureFromImage(LoadImage("img/TETRIS2.png"));
    tetrisImage3 = LoadTextureFromImage(LoadImage("img/TETRIS3.png"));
    homeImage1 = LoadTextureFromImage(LoadImage("img/letplay.png"));
    homeImage2 = LoadTextureFromImage(LoadImage("img/howtoplay.png"));
    homeImage3 = LoadTextureFromImage(LoadImage("img/leaderboard.png"));
    homeImage4 = LoadTextureFromImage(LoadImage("img/home.png"));
    trophyImage = LoadTextureFromImage(LoadImage("img/trophy.png"));
    gameOverOn = LoadTextureFromImage(LoadImage("img/GameOverOn.png"));
    gameOverOff = LoadTextureFromImage(LoadImage("img/GameOverOff.png"));
    breakRecord = LoadTextureFromImage(LoadImage("img/BreakTheRecord.png"));
    soundOn = LoadTextureFromImage(LoadImage("img/SoundOn.png"));
    soundOff = LoadTextureFromImage(LoadImage("img/SoundOff.png"));
    aboutUs = LoadTextureFromImage(LoadImage("img/About.png"));
    guide = LoadTextureFromImage(LoadImage("img/Guide.png"));
    logo = LoadTextureFromImage(LoadImage("img/TETRISsmall.png"));
    aboutUsNormal = LoadTextureFromImage(LoadImage("img/AboutUsNormal.png"));
    aboutUsHover = LoadTextureFromImage(LoadImage("img/AboutUsHover.png"));
}

void Game::UnloadGameTexture()
{
    UnloadTexture(tetrisImage1);
    UnloadTexture(tetrisImage2);
    UnloadTexture(tetrisImage3);
    UnloadTexture(homeImage1);
    UnloadTexture(homeImage2);
    UnloadTexture(homeImage3);
    UnloadTexture(homeImage4);
    UnloadTexture(trophyImage);
    UnloadTexture(gameOverOn);
    UnloadTexture(gameOverOff);
    UnloadTexture(breakRecord);
    UnloadTexture(soundOn);
    UnloadTexture(soundOff);
    UnloadTexture(aboutUs);
    UnloadTexture(leaderBoard);
    UnloadTexture(guide);
    UnloadTexture(logo);
    UnloadTexture(aboutUsNormal);
    UnloadTexture(aboutUsHover);
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
        getLeaderBoard();
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
    FILE *file = fopen("scores.txt", "a");
    if (file == NULL)
    {
        return;
    }
    char name[namePlayer.size()];
    strcpy(name, namePlayer.c_str());
    if (strcmp(name, "") != 0)
    {
        fprintf(file, "%s-", namePlayer.c_str());
        fprintf(file, "%d-", score);
        fprintf(file, "%d\n", timePlayed);
    }
    fclose(file);
}

void Game::Guide(int &choice)
{
    ButtonO btn3(" ", 1020, 20, font1, 60, 6); // short home

    while (!WindowShouldClose())
    {
        if (!isMute)
            UpdateMusicStream(music);
        BeginDrawing();
        ClearBackground(backgroundColor);
        btn3.draw();
        DrawTexture(homeImage4, 1065, 27, WHITE);
        DrawTextEx(font1, "HOW TO PLAY", {300, 90}, 100, 5, PINK);
        DrawTexture(guide, 100, 200, WHITE);
        EndDrawing();

        if (btn3.update() == MOUSE_BUTTON_LEFT)
        {
            choice = 3;
            break;
        }
    }
}

void  Game::getLeaderBoard() {
    topPlayers.clear();
    FILE *file = fopen("scores.txt", "r");
    if (file == NULL)
    {
        return;
    }
    char name[10];
    int score;
    int timePlayed;
    while (fscanf(file, "%[^-]-%d-%d\n", name, &score, &timePlayed) != EOF)
    {
        Player player;
        strcpy(player.name, name);
        player.score = score;
        player.timePlayed = timePlayed;
        topPlayers.push_back(player);
    }
    fclose(file);
    std::sort(topPlayers.begin(), topPlayers.end(), [](const Player& a, const Player& b) {
        if (a.score == b.score)
            return a.timePlayed < b.timePlayed;
        return a.score > b.score;
    });
    FILE *file2 = fopen("scores.txt", "w");
    if (file2 == NULL)
    {
        return;
    }
    for (int i = 0; i < topPlayers.size() && i < 3; i++)
    {
        fprintf(file2, "%s-", topPlayers[i].name);
        fprintf(file2, "%d-", topPlayers[i].score);
        fprintf(file2, "%d\n", topPlayers[i].timePlayed);
    }
    fclose(file2);
}

void PrintTopPlayer(const Player& p, float PosY, Font f, Color c)
{
    char nameText[10];
    char scoreText[10];
    char timePlayedText[10];

    sprintf(nameText, "%s", p.name);
    DrawTextEx(f, nameText, {350, PosY}, 50, 5, c);
    sprintf(scoreText, "%d", p.score);
    DrawTextEx(f, scoreText, {650, PosY}, 60, 5, c);
    sprintf(timePlayedText, "%d", p.timePlayed);
    DrawTextEx(f, timePlayedText, {900, PosY}, 60, 5, c);
}

void Game::LeaderBoard(int &choice)
{
    vector<float> PosY = {300, 450, 600};
    vector<Color> c = {RED, {26, 31, 40, 255}, {255, 107, 1, 255}};
    ButtonO btn3(" ", 1020, 20, font1, 60, 6);

    getLeaderBoard();
    while (!WindowShouldClose())
    {
        if (!isMute)
            UpdateMusicStream(music);
        BeginDrawing();
        ClearBackground(Color{43, 39, 57, 1});
        btn3.draw();

        DrawTexture(homeImage4, 1065, 27, WHITE);
        DrawTextEx(font1, "LEADERBOARD", {360, 80}, 80, 5, PINK);
        DrawTexture(trophyImage, 100, 200, WHITE);
        DrawTextEx(font1, "Name", {350, 215}, 40, 5, BLACK);
        DrawTextEx(font1, "Score", {650, 215}, 40, 5, BLACK);
        DrawTextEx(font1, "Time", {900, 215}, 40, 5, BLACK);

        for (int i = 0; i < topPlayers.size() && i < 3; i++)
            PrintTopPlayer(topPlayers[i], PosY[i], font1, c[i]);
        EndDrawing();

        if (btn3.update() == MOUSE_BUTTON_LEFT)
        {
            choice = 3;
            break;
        }
    }
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

void Game::Over(int &choice)
{
    ButtonO btn3(" Play again ", 350, 420, font1, 60, 4);
    ButtonO btn5("  Home  ", 400, 600, font1, 60, 4);

    while (!WindowShouldClose())
    {
        if (!isMute)
            UpdateMusicStream(music);
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
}

void Game::InfoGroup(int &choice)
{
    ButtonO btn3(" ", 1020, 20, font1, 60, 6); // short home

    while (!WindowShouldClose())
    {
        if (!isMute)
            UpdateMusicStream(music);
        BeginDrawing();

        if (btn3.isHover() == true)
        {
            ClearBackground(backgroundColor);
            btn3.draw();
            DrawTexture(homeImage4, 1065, 27, WHITE);
            DrawTextEx(font1, "About Us", {100, 20}, 150, 5, PINK);
            DrawTexture(aboutUsHover, 0, 167, WHITE);
        }

        else
        {
            ClearBackground(WHITE);
            btn3.draw();
            DrawTexture(homeImage4, 1065, 27, WHITE);
            DrawTextEx(font1, "About Us", {100, 20}, 150, 5, {148, 82, 255, 255});
            DrawTexture(aboutUsNormal, 0, 167, WHITE);
        }

        EndDrawing();

        if (btn3.update() == MOUSE_BUTTON_LEFT)
        {
            choice = 3;
            break;
        }
    }
}
void Game::CountDown()
{
    float numPosX = wWidth / 2 + 10;
    float numPosY = wHeight / 2 - 50;

    auto start = std::chrono::steady_clock::now();
    int countTime = 3;

    while (!WindowShouldClose() && countTime > -1)
    {
        if (!isMute)
            UpdateMusicStream(music);
        BeginDrawing();
        ClearBackground(backgroundColor);
        GameInfo();

        char countDown[10];
        sprintf(countDown, "%d", countTime);
        DrawTextEx(font, countDown, {numPosX, numPosY}, 150, 4, YELLOW);
        EndDrawing();

        auto end = std::chrono::steady_clock::now();
        countTime = std::chrono::duration_cast<chrono::seconds>(end - start).count();
        countTime = 3 - countTime;
    }
}

void Game::GameInfo()
{
    DrawTextEx(font, "Score", {100, 440}, 40, 5, PINK);
    DrawTextEx(font, "Next", {850, 99}, 40, 5, PINK);
    DrawTextEx(font, "Line", {900, 440}, 40, 5, PINK);
    DrawTextEx(font, "Player", {100, 200}, 40, 5, PINK);
    DrawTextEx(font, "Time", {900, 600}, 40, 5, PINK);
    DrawTexture(logo, 50, 40, WHITE);

    char timePlayedText[10];
    sprintf(timePlayedText, "%d", timePlayed);
    DrawTextEx(font, timePlayedText, {900, 660}, 70, 5, WHITE);

    char namePlayedText[10];
    sprintf(namePlayedText, "%s", namePlayer.c_str());
    DrawTextEx(font, namePlayedText, {100, 260}, 70, 5, WHITE);

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
    auto start = std::chrono::steady_clock::now();
    while (!WindowShouldClose() && choice != 3 && choice != 6)
    {
        BeginDrawing();
        ClearBackground(backgroundColor);
        if (!isMute)
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
    ButtonO btn0("        Let's Play      ", 110, 350, font1, 50, 4);
    ButtonO btn1(" How To Play     ", 500, 475, font1, 50, 4);
    ButtonO btn2("     Leader Board ", 110, 600, font1, 50, 4);
    ButtonO btnSound(" ", 1020, 600, font1, 60, 6);
    ButtonO btnAbout(" ", 1020, 700, font1, 60, 6);

    while (!WindowShouldClose())
    {
        if (!isMute)
            UpdateMusicStream(music);
        BeginDrawing();
        ClearBackground(Color{43, 39, 57, 1});

        btn0.draw();
        btn1.draw();
        btn2.draw();
        btnSound.draw();
        btnAbout.draw();

        if (btnSound.update() == MOUSE_BUTTON_LEFT)
            isMute = !isMute;
        if (isMute)
            DrawTexture(soundOff, 1050, 610, WHITE);
        else
            DrawTexture(soundOn, 1050, 610, WHITE);

        DrawTexture(aboutUs, 1050, 700, WHITE);
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
        if (btnAbout.update() == MOUSE_BUTTON_LEFT)
        {
            choice = 7;
            break;
        }
    }
}

void Game::GetReady(int &choice)
{
    ButtonO btn4(" ", 1020, 20, font1, 60, 6); // HOME Ngắn
    ButtonO btn5("Start", 490, 600, font1, 60, 4);
    Rectangle textBox = {200, 400, 800, 100};

    bool mouseOnText = 1;
    int framesCounter = 0;
    int letterCount = 0;
    int MAX_INPUT_CHARS = 9;

    while (!WindowShouldClose())
    {
        if (!isMute)
            UpdateMusicStream(music);
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
        else if (choice == 7)
        {
            InfoGroup(choice);
        }
    }
}
