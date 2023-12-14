#include <raylib.h>
#include "game.h"
#include "colors.h"
#include <iostream>
#include "myGui.hpp"
double lastUpdateTime = 0;

int main()
{
    const int screenWidth = 1100;
    const int screenHeight = 950;
    int lose = 0;
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(screenWidth, screenHeight, "My Button Classes");

    SetTargetFPS(60);
    InitAudioDevice();
    int choice = 1;
    Font font1 = LoadFont("resources/fonts/monogram.ttf");
    Font font2 = LoadFont("resources/fonts/segoeui.ttf");
    ButtonO btn1("Let's Play", 100, 200, font1);
    ButtonO btn2("How To Play", 100, 400, font1);
    ButtonO btn3("Leader Board", 100, 600, font1);
    ButtonO btn4("Home", 500, 200, font1);
    ButtonO btn5("Start", 100, 600, font1);
    ButtonO btn6("PLAY AGAIN", 100, 300, font1);
    ButtonO btn7("IF LOSE", 100, 400, font1);
    while (!WindowShouldClose())
    {
       

        if (choice == 2)
        {
            BeginDrawing();
            ClearBackground(RED);
            DrawTextEx(font1, "GET READY", {839, 417}, 40, 5, PINK);
            btn4.draw();
            btn5.draw();
            EndDrawing();
            if (btn4.update() == MOUSE_BUTTON_LEFT)
                choice = 1;
            if (btn5.update() == MOUSE_BUTTON_LEFT)
                choice = 5;
        }
        else if (choice == 1)
        {
            BeginDrawing();
            ClearBackground(WHITE);
            DrawTextEx(font1, "HOME", {839, 417}, 40, 5, PINK);
            btn1.draw();
            btn2.draw();
            btn3.draw();
            EndDrawing();
            if (btn1.update() == MOUSE_BUTTON_LEFT)
                choice = 2;
            if (btn2.update() == MOUSE_BUTTON_LEFT)
                choice = 3;
            if (btn3.update() == MOUSE_BUTTON_LEFT)
                choice = 4;
        }

        else if (choice == 3)
        {
            BeginDrawing();
            ClearBackground(YELLOW);
            btn4.draw();
            DrawTextEx(font1, "HOW TO PLAY", {839, 417}, 40, 5, PINK);
            EndDrawing();
            if (btn4.update() == MOUSE_BUTTON_LEFT)
                choice = 1;
        }

        else if (choice == 4)
        {
            BeginDrawing();
            ClearBackground(PINK);
            btn4.draw();
            DrawTextEx(font1, "LEADERBOARD", {839, 417}, 40, 5, BLACK);
            EndDrawing();
            if (btn4.update() == MOUSE_BUTTON_LEFT)
                choice = 1;
        }

        else if (choice == 5)
        {
            BeginDrawing();
            ClearBackground(BLUE);
            btn4.draw();
            btn7.draw();
            DrawTextEx(font1, "PLAY GAME", {839, 417}, 40, 5, PINK);
            EndDrawing();
            if (btn4.update() == MOUSE_BUTTON_LEFT)
                choice = 1;
            if (btn7.update() == MOUSE_BUTTON_LEFT)
            {
                lose = 1;
                choice = 6;
            }
        }

        else if (lose == 1 && choice == 6)
        {
            BeginDrawing();
            ClearBackground(ORANGE);
            btn4.draw();
            btn6.draw();
            DrawTextEx(font1, "GAME OVER", {839, 417}, 40, 5, PINK);
            EndDrawing();
            if (btn4.update() == MOUSE_BUTTON_LEFT)
                choice = 1;
            if (btn6.update() == MOUSE_BUTTON_LEFT)
                choice = 2;
        }
    }
    CloseWindow();
    return 0;
}
