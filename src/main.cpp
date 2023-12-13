#include <raylib.h>
#include "game.h"
#include "colors.h"
#include <iostream>

double lastUpdateTime = 0;

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

double updateInterval(Game &game)
{
    int linesCleared = game.linesCleared;
    double interval = 0.5;
    interval = interval - (linesCleared / 5) * 0.1 > 0.2 ? interval - (linesCleared / 5) * 0.1 : 0.2;
    return interval;
}
int main()
{
    InitWindow(1200, 800, "Tetris");
    SetTargetFPS(60);

    Font font = LoadFontEx("font/Inter-Bold.ttf", 64, 0, 0);
    Game game = Game();

    while (WindowShouldClose() == false)
    {
        BeginDrawing();
        ClearBackground(Color{43, 39, 57, 1});

        UpdateMusicStream(game.music);
        game.HandleInput();
        if (EventTriggered(updateInterval(game)))
        {
            game.MoveBlockDown();
        }

        if (game.gameOver)
        {
            DrawTextEx(font, "Game Over", {200, 400}, 40, 5, PINK);
            DrawTextEx(font, "Press R to restart", {200, 500}, 40, 5, PINK);
            lastUpdateTime = 0;
        }
        else
        {
            DrawTextEx(font, "Score", {839, 417}, 40, 5, PINK);
            DrawTextEx(font, "Next", {833, 99}, 40, 5, PINK);
            DrawTextEx(font, "Line", {370, 460}, 40, 5, PINK);
            DrawTextEx(font, "Time", {370, 99}, 40, 5, PINK);
            DrawTextEx(font, "Interval", {100, 99}, 40, 5, PINK);   

            char intervalText[10];
            sprintf(intervalText, "%.1f", updateInterval(game));
            DrawTextEx(font, intervalText, {100, 146}, 40, 5, WHITE);

            char timePlayedText[10];
            sprintf(timePlayedText, "%d", (int)lastUpdateTime);
            DrawTextEx(font, timePlayedText, {400, 146}, 40, 5, WHITE);

            DrawRectangleRounded({863, 482, 250, 100}, 1, 6, lightBlue);
            char scoreText[10];
            sprintf(scoreText, "%d", game.score);
            Vector2 textSize = MeasureTextEx(font, scoreText, 38, 2);
            DrawTextEx(font, scoreText, {863 + (250 - textSize.x) / 2, 493}, 40, 5, WHITE);

            char linesClearedText[10];
            sprintf(linesClearedText, "%d", game.linesCleared);
            DrawTextEx(font, linesClearedText, {400, 511}, 40, 5, WHITE);

            DrawRectangleRounded({863, 146, 250, 200}, 0.5, 6, lightBlue);
            game.Draw();
        }
        EndDrawing();
    }
    CloseWindow();
}