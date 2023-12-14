#include <raylib.h>
#include "game.h"
#include "colors.h"

#define MAX_INPUT_CHARS 9
#define W 1200
#define H 800

double lastUpdateTime = 0;

int main()
{
    InitWindow(W, H, "Tetris");
    SetTargetFPS(60);

    Game game = Game();

    game.Home();

    game.GetReady();
    game.Play(lastUpdateTime);
    
    CloseWindow();
}