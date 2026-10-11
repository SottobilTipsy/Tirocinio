#include <raylib.h>

Color BackgroundGray = {30,30,30,255};

void tempgame()
{
    InitWindow(1820,1000, "Knotangle");
    SetTargetFPS(60);
    while(WindowShouldClose() == false)
    {
        BeginDrawing();
        ClearBackground(BackgroundGray);
        EndDrawing();
    }
    CloseWindow();
}