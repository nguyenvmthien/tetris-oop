#include <raylib.h>
#include "game.h"
#include "colors.h"
#include <iostream>
#define MAX_INPUT_CHARS 9
#define W 1200
#define H 800

double lastUpdateTime = 0;
bool nameEntered = false;

bool EventTriggered(double interval)
{
    double currentTime = GetTime();
    if (currentTime - lastUpdateTime >= interval)
    {
        lastUpdateTime = currentTime;
        return true;
    }
    return false;
}

double updateInterval(const Game &game)
{
    int linesCleared = game.linesCleared;
    double interval = 0.5;
    interval = interval - (linesCleared / 5) * 0.1 > 0.2 ? interval - (linesCleared / 5) * 0.1 : 0.2;
    return interval;
}

void GetReady(int& isReady, float& numPosX, float& readyPosX, Font& font, Color& background)
{
    if (isReady < 50)
    {
        DrawTextEx(font, "Get Ready", {readyPosX, 50}, 50, 4, YELLOW);
    }
    else if (isReady < 100)
    {
        DrawTextEx(font, "Get Ready", {readyPosX, 50}, 50, 4, background);
        DrawTextEx(font, "3", {numPosX, 50}, 50, 4, YELLOW);
    }
    else if (isReady < 150)
    {
        DrawTextEx(font, "3", {numPosX, 50}, 50, 4, background);
        DrawTextEx(font, "2", {numPosX, 50}, 50, 4, YELLOW);
    }
    else if (isReady < 200)
    {
        DrawTextEx(font, "2", {numPosX, 50}, 50, 4, background);
        DrawTextEx(font, "1", {numPosX, 50}, 50, 4, YELLOW);
    }
    else if (isReady < 250)
    {
        DrawTextEx(font, "1", {numPosX, 50}, 50, 4, background);
        DrawTextEx(font, "0", {numPosX, 50}, 50, 4, YELLOW);
    }
    else
    {
        DrawTextEx(font, "0", {numPosX, 50}, 50, 4, background);
    }
    isReady++;
}

void Over(Font& font)
{
    DrawTextEx(font, "Game Over", {200, 400}, 40, 5, PINK);
    DrawTextEx(font, "Press R to restart", {200, 500}, 40, 5, PINK);
    lastUpdateTime = 0;
}

void GameInfo(Game& game, Font& font)
{
    DrawTextEx(font, "Score", {839, 417}, 40, 5, PINK);
    DrawTextEx(font, "Next", {833, 99}, 40, 5, PINK);
    DrawTextEx(font, "Line", {370, 460}, 40, 5, PINK);
    DrawTextEx(font, "Name", {370, 99}, 40, 5, PINK);
    DrawTextEx(font, "Interval", {100, 99}, 40, 5, PINK);   

    char intervalText[10];
    sprintf(intervalText, "%.1f", updateInterval(game));
    DrawTextEx(font, intervalText, {100, 146}, 40, 5, WHITE);

    char namePlayedText[10];
    sprintf(namePlayedText, "%d", game.namePlayer.c_str());
    DrawTextEx(font, namePlayedText, {400, 146}, 40, 5, WHITE);

    // DrawRectangleRounded({863, 482, 250, 100}, 1, 6, lightBlue);
    char scoreText[10];
    sprintf(scoreText, "%d", game.score);
    Vector2 textSize = MeasureTextEx(font, scoreText, 38, 2);

    DrawTextEx(font, scoreText, {839 + 55 - textSize.x/2, 460}, 40, 5, WHITE);

    char linesClearedText[10];
    sprintf(linesClearedText, "%d", game.linesCleared);
    DrawTextEx(font, linesClearedText, {400, 511}, 40, 5, WHITE);

    DrawRectangleRounded({863, 146, 250, 200}, 0.5, 6, darkGrey);
    game.Draw();
}

int main()
{
    InitWindow(W, H, "Tetris");
    SetTargetFPS(60);

    Font font = LoadFontEx("Font/Inter-Bold.ttf", 64, 0, 0);
    Game game = Game();

    int letterCount = 0;
    Rectangle textBox = {600, 400, 255, 50};
    bool mouseOnText = false;
    int framesCounter = 0;
    
    Image image = LoadImage("img/TETRIS.png");
    Texture2D tetrisImage = LoadTextureFromImage(image);
    UnloadImage(image);

    while (!nameEntered && !WindowShouldClose())
    {
        if (CheckCollisionPointRec(GetMousePosition(), textBox))
            mouseOnText = true;
        else
            mouseOnText = false;

        if (mouseOnText)
        {
            SetMouseCursor(MOUSE_CURSOR_IBEAM);

            int key = GetCharPressed();

            while (key > 0)
            {
                if ((key >= 32) && (key <= 125) && (letterCount < MAX_INPUT_CHARS))
                {
                    game.namePlayer[letterCount] = (char)key;
                    game.namePlayer[letterCount + 1] = '\0';
                    letterCount++;
                }

                key = GetCharPressed();
            }

            if (IsKeyPressed(KEY_BACKSPACE))
            {
                letterCount--;
                if (letterCount < 0)
                    letterCount = 0;
                game.namePlayer[letterCount] = '\0';
            }
        }
        else
            SetMouseCursor(MOUSE_CURSOR_DEFAULT);
        if (mouseOnText)
            framesCounter++;
        else
            framesCounter = 0;
        BeginDrawing();
        ClearBackground(Color{43, 39, 57, 1});
        DrawTexture(tetrisImage, 350, 100, WHITE);
        DrawTextEx(font, "Enter your name", {200, 400}, 40, 5, PINK);
        DrawTextEx(font, "Press Enter to start", {200, 500}, 40, 5, PINK);
        DrawTextEx(font, "Press H for help", {200, 600}, 40, 5, PINK);
        if (IsKeyPressed(KEY_H))
        {
            game.HuongDan();
        }
        
        if (IsKeyPressed(KEY_ENTER))
        {
            game.namePlayer = game.namePlayer;
            break;
        }
        DrawRectangleRec(textBox, LIGHTGRAY);
        if (mouseOnText)
            DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, RED);
        else
            DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, DARKGRAY);

        DrawText(game.namePlayer.c_str(), (int)textBox.x + 5, (int)textBox.y + 8, 40, MAROON);

        if (mouseOnText)
        {
            if (letterCount < MAX_INPUT_CHARS)
            {
                if (((framesCounter / 20) % 2) == 0)
                    DrawText("_", (int)textBox.x + 8 + MeasureText(game.namePlayer.c_str(), 40), (int)textBox.y + 12, 40, MAROON);
            }
        }
        EndDrawing();
    }

    // Font font = LoadFontEx("font/Inter-Bold.ttf", 64, 0, 0);
    // Game game = Game();
    Color background = {43, 39, 57, 1};

    float numPosX = W/2 + 20;
    float readyPosX = W/2 - 70;

    while (WindowShouldClose() == false)
    {
        BeginDrawing();
        ClearBackground(background);
        UpdateMusicStream(game.music);

        if (game.isReady < 300)
            GetReady(game.isReady, numPosX, readyPosX, font, background);
        else 
        {
            game.HandleInput();
            if (EventTriggered(updateInterval(game)))
            {
                game.MoveBlockDown();
            }
        }
        if (game.gameOver)
        {
            Over(font);
        }
        else
        {
            GameInfo(game, font);
        }
        EndDrawing();
    }
    UnloadTexture(tetrisImage);
    CloseWindow();
}