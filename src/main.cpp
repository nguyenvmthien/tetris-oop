#include <raylib.h>
#include "game.h"
#include "colors.h"
#include <iostream>

#define MAX_INPUT_CHARS 9
#define W 1200
#define H 800

double lastUpdateTime = 0;
bool nameEntered = false;

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
    game.Play(lastUpdateTime);
    
    UnloadTexture(tetrisImage);
    CloseWindow();
}