#include <raylib.h>
#include "game.h"
#include "colors.h"
#include <iostream>
#include "myGui.hpp"
double lastUpdateTime = 0;
// chinh constructor ButtonO
//  chinh thong so
// chinh thong so ô nhập tên
int main()
{
    const int screenWidth = 1200;
    const int screenHeight = 800;
    int lose = 0;
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(screenWidth, screenHeight, "My Button Classes");

    SetTargetFPS(60);
    InitAudioDevice();
    int choice = 1;
    bool is_mute = false;

    Font font1 = LoadFont("resources/fonts/monogram.ttf");

    Image image1 = LoadImage("img/TETRIS1.png");
    Image image2 = LoadImage("img/TETRIS2.png");
    Image image3 = LoadImage("img/TETRIS3.png");
    Image letplay = LoadImage("img/letplay.png");
    Image howtoplay = LoadImage("img/howtoplay.png");
    Image leaderboard = LoadImage("img/leaderboard.png");
    Image home = LoadImage("img/home.png");
    Image trophy = LoadImage("img/trophy.png");
    Image gameover1 = LoadImage("img/GameOverOn.png");
    Image gameover2 = LoadImage("img/GameOverOff.png");
    Image breakrecord = LoadImage("img/BreakTheRecord.png");
    Image sound1 = LoadImage("img/SoundOn.png");
    Image sound2 = LoadImage("img/SoundOff.png");
    Image about = LoadImage("img/About.png");

    Texture2D tetrisImage1 = LoadTextureFromImage(image1);
    Texture2D tetrisImage2 = LoadTextureFromImage(image2);
    Texture2D tetrisImage3 = LoadTextureFromImage(image3);
    Texture2D homeImage1 = LoadTextureFromImage(letplay);
    Texture2D homeImage2 = LoadTextureFromImage(howtoplay);
    Texture2D homeImage3 = LoadTextureFromImage(leaderboard);
    Texture2D homeImage4 = LoadTextureFromImage(home);
    Texture2D trophyImage = LoadTextureFromImage(trophy);
    Texture2D gameOverOn = LoadTextureFromImage(gameover1);
    Texture2D gameOverOff = LoadTextureFromImage(gameover2);
    Texture2D breakRecord = LoadTextureFromImage(breakrecord);
    Texture2D soundOn = LoadTextureFromImage(sound1);
    Texture2D soundOff = LoadTextureFromImage(sound2);
    Texture2D aboutUs = LoadTextureFromImage(about);

    UnloadImage(trophy);
    UnloadImage(image1);
    UnloadImage(image2);
    UnloadImage(image3);
    UnloadImage(letplay);
    UnloadImage(howtoplay);
    UnloadImage(leaderboard);
    UnloadImage(home);
    UnloadImage(gameover1);
    UnloadImage(gameover2);
    UnloadImage(breakrecord);
    UnloadImage(sound1);
    UnloadImage(sound2);
    UnloadImage(about);

    ButtonO btn1("        Let's Play      ", 110, 350, font1, 50, 4);
    ButtonO btn2(" How To Play     ", 500, 475, font1, 50, 4);
    ButtonO btn3("     Leader Board ", 110, 600, font1, 50, 4);
    ButtonO btn4("", 1020, 20, font1, 60, 6); // HOME Ngắn
    ButtonO btn8("   Home  ", 350, 600, font1, 60, 4);
    ButtonO btn5("Start", 490, 600, font1, 60, 4);
    ButtonO btn6(" Play again ", 350, 400, font1, 60, 4);
    ButtonO btn7("IF LOSE", 100, 400, font1, 50, 4);
    ButtonO btnSound("", 1020, 600, font1, 60, 6);
    ButtonO btnAbout("", 1020, 700, font1, 60, 6);

    while (!WindowShouldClose())
    {

        if (choice == 2)
        {
            BeginDrawing();
            ClearBackground({43, 39, 57, 1});
            Rectangle textBox = {200, 400, 800, 100};
            DrawRectangleRec(textBox, LIGHTGRAY);
            DrawTextEx(font1, "Enter your name", {200, 350}, 30, 5, PINK);
            DrawTextEx(font1, "Maximum 9 characters", {600, 510}, 30, 5, PINK);
            DrawTextEx(font1, "GET READY", {450, 50}, 60, 5, PINK);
            btn4.draw();
            btn5.draw();
            DrawTexture(homeImage4, 1065, 27, WHITE);
            if (btn5.isHover() == true || btn4.isHover() == true)
                DrawTexture(tetrisImage1, 200, 100, WHITE);
            else
                DrawTexture(tetrisImage2, 200, 100, WHITE);
            EndDrawing();
            if (btn4.update() == MOUSE_BUTTON_LEFT)
                choice = 1;
            if (btn5.update() == MOUSE_BUTTON_LEFT)
                choice = 5;
        }
        else if (choice == 1)
        {
            BeginDrawing();
            ClearBackground({43, 39, 57, 1});
            if (btn1.isHover() == true || btn2.isHover() == true || btn3.isHover() == true)
                DrawTexture(tetrisImage1, 200, 100, WHITE);
            else
                DrawTexture(tetrisImage2, 200, 100, WHITE);

            DrawTextEx(font1, "HOME", {500, 50}, 60, 5, PINK);
            btn1.draw();
            btn2.draw();
            btn3.draw();
            btnSound.draw();
            btnAbout.draw();
            if (btnSound.update() == MOUSE_BUTTON_LEFT)
                is_mute = !is_mute;
            if (is_mute == false)
                DrawTexture(soundOn, 1050, 610, WHITE);
            else
                DrawTexture(soundOff, 1050, 610, WHITE);
            DrawTexture(aboutUs, 1050, 700, WHITE);
            DrawTexture(homeImage1, 200, 352, WHITE);
            DrawTexture(homeImage2, 900, 474, WHITE);
            DrawTexture(homeImage3, 150, 603, WHITE);

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
            Image image = LoadImage("img/huongdanchoi.png");
            Texture2D huongdanImage = LoadTextureFromImage(image);
            UnloadImage(image);
            BeginDrawing();
            ClearBackground(Color{43, 39, 57, 1});
            btn4.draw();
            DrawTexture(homeImage4, 1065, 27, WHITE);
            DrawTextEx(font1, "HOW TO PLAY", {400, 100}, 40, 5, PINK);
            DrawTexture(huongdanImage, 100, 200, WHITE);
            EndDrawing();
            if (btn4.update() == MOUSE_BUTTON_LEFT)
                choice = 1;
        }

        else if (choice == 4)
        {
            BeginDrawing();
            ClearBackground(Color{43, 39, 57, 1});
            btn4.draw();
            DrawTexture(homeImage4, 1065, 27, WHITE);
            DrawTextEx(font1, "LEADERBOARD", {350, 100}, 80, 5, PINK);
            DrawTexture(trophyImage, 100, 200, WHITE);
            DrawTextEx(font1, "Name", {400, 215}, 40, 5, BLACK);
            DrawTextEx(font1, "Score", {650, 215}, 40, 5, BLACK);
            DrawTextEx(font1, "Time", {900, 215}, 40, 5, BLACK);

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
            DrawTextEx(font1, "PLAY GAME", {400, 100}, 40, 5, PINK);
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
            ClearBackground(Color{43, 39, 57, 1});
            btn6.draw();
            btn8.draw();
            DrawTextEx(font1, "Score: ", {400, 300}, 60, 8, PINK);
            if (btn6.isHover() == true)
                DrawTexture(breakRecord, 0, 50, WHITE);
            else if (btn8.isHover() == true)
                DrawTexture(gameOverOn, 180, 50, WHITE);
            else
                DrawTexture(gameOverOff, 180, 50, WHITE);
            EndDrawing();
            if (btn6.update() == MOUSE_BUTTON_LEFT)
                choice = 5;
            if (btn8.update() == MOUSE_BUTTON_LEFT)
                choice = 1;
        }
    }
    CloseWindow();
    return 0;
}
