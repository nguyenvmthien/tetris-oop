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

int main()
{
    InitWindow(1200, 800, "Tetris");
    SetTargetFPS(60);

    Font font = LoadFontEx("Font/Inter-Bold.ttf", 64, 0, 0);

    Game game = Game();

    while (WindowShouldClose() == false)
    {
        UpdateMusicStream(game.music);
        game.HandleInput();
        if (EventTriggered(0.2))
        {
            game.MoveBlockDown();
        }

        BeginDrawing();
        ClearBackground(Color{43, 39, 57, 1});
        DrawTextEx(font, "Score", {839, 417}, 40, 5, PINK);
        DrawTextEx(font, "Next", {833, 99}, 40, 5, PINK);
        DrawTextEx(font, "Line", {370, 460}, 40, 5, PINK);
        if (game.gameOver)
        {
            DrawTextEx(font, "GAME OVER", {150, 350}, 50, 5, PINK);
        }

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
        EndDrawing();
    }
    CloseWindow();
}