#include <raylib.h>
#include "temp.hpp"
#include "Nbox.hpp"
#include "Button.hpp"
#include "grid.hpp"
#include "Gridmaker.hpp"
#include "knotbox.hpp"
#include "Checkbox.hpp"

Color BackgroundGray = {30,30,30,255};
Color TitlePurple = {100,10,170,255};
Color TitlePurpleOutlines = {150,60,220,255};

void tempgame()
{
    InitWindow(1800,1000, "Knotangle");
    SetExitKey(KEY_NULL); 
    SetTargetFPS(60);
    Texture2D MenuBG = LoadTexture("Resources/MenuBG.jpeg");
    Font TitleFont = LoadFont("Resources/Debrosee-ALPnL.ttf");
    Nbox input_number_box = Nbox(0,400,false,false,TitleFont);
    Knotbox knotbox = Knotbox();
    grid finalgrid = grid();

    bool ConfirmPressed = false;
    bool showmenu = true;
    bool Algorithmdone = false;

    Camera2D camera;
    camera.offset = {0,0};
    camera.target = {0,0};
    camera.rotation = 0;
    camera.zoom = 1;

    while(WindowShouldClose() == false)
    {
        bool Mousepressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
        BeginDrawing();
        ClearBackground(BackgroundGray);

        if (showmenu)
        {
            DrawTexture(MenuBG,0,0,WHITE);
            DrawTextEx(TitleFont,"KNOTANGLE",{607,87},120,5,TitlePurpleOutlines);
            DrawTextEx(TitleFont,"KNOTANGLE",{600,80},120,5,TitlePurple);
            input_number_box.Draw(TitleFont);
            if (input_number_box.get_button().IsPressed(Mousepressed))
                showmenu = false; 
        }
        else
        {
            if (!Algorithmdone)
            {
                Gridmaker Gridmaker;
                finalgrid = Gridmaker.grid_generator(input_number_box.get_checkbox().get_size_from_text());
                knotbox = Knotbox(finalgrid);
                Algorithmdone = true;
            }

            BeginMode2D(camera);

            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
            {
                Vector2 delta = GetMouseDelta();
                camera.target.x -= delta.x / camera.zoom;
                camera.target.y -= delta.y / camera.zoom;
                SetMouseCursor(MOUSE_CURSOR_RESIZE_ALL);
            }
            else
            {
                SetMouseCursor(MOUSE_CURSOR_DEFAULT);
            }
            
            float wheel = GetMouseWheelMove();
            if (wheel != 0.0f)
            {
                Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);          
                camera.offset = GetMousePosition();          
                camera.target = mouseWorldPos;
                camera.zoom += wheel * 0.15;
                if (camera.zoom < 0.05f) camera.zoom = 0.05f;
                if (camera.zoom > 5.0f) camera.zoom = 5.0f;
            }
            knotbox.Handler(camera);
            knotbox.Draw();
            EndMode2D();
        }
        EndDrawing();
    }
    CloseWindow();
}