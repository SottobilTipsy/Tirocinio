#include "raylib.h"

int main(void)
{
    // 1. Window Initialization
    const int screenWidth = 1800;
    const int screenHeight = 1000;
    InitWindow(screenWidth, screenHeight, "Raylib Zoomable & Pannable Grid");

    // 2. Grid & World Parameters
    const int gridCellsX = 500;
    const int gridCellsY = 500;
    const int tileSize = 10;
    const int worldWidth = gridCellsX * tileSize;   // 5000 pixels
    const int worldHeight = gridCellsY * tileSize;  // 5000 pixels

    // 3. Camera Setup
    Camera2D camera = { 0 };
    camera.target = (Vector2){ (float)worldWidth / 2.0f, (float)worldHeight / 2.0f };
    camera.offset = (Vector2){ (float)screenWidth / 2.0f, (float)screenHeight / 2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 0.25f;

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
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

        // --- ZOOM LOGIC (Mouse Wheel) ---
        float wheel = GetMouseWheelMove();
        if (wheel != 0.0f)
        {
            // Get the world point currently under the mouse
            Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);

            // Set camera offset to the current mouse screen position
            camera.offset = GetMousePosition();

            // Set target to that world position so it zooms directly toward the cursor
            camera.target = mouseWorldPos;

            // Adjust zoom level
            camera.zoom += wheel * 1;
            
            // Clamp zoom limits
            if (camera.zoom < 0.05f) camera.zoom = 0.05f;
            if (camera.zoom > 5.0f) camera.zoom = 5.0f;
        }

        // --- RESET CAMERA ('R' Key) ---
        if (IsKeyPressed(KEY_R))
        {
            camera.target = (Vector2){ (float)worldWidth / 2.0f, (float)worldHeight / 2.0f };
            camera.offset = (Vector2){ (float)screenWidth / 2.0f, (float)screenHeight / 2.0f };
            camera.zoom = 0.25f;
        }

        // --- DRAWING ---
        BeginDrawing();
            ClearBackground(RAYWHITE);

            BeginMode2D(camera);

                // Draw background of the grid area
                DrawRectangle(0, 0, worldWidth, worldHeight, (Color){ 240, 240, 240, 255 });

                // Draw grid lines
                for (int x = 0; x <= gridCellsX; x++)
                {
                    DrawLine(x * tileSize, 0, x * tileSize, worldHeight, LIGHTGRAY);
                }
                for (int y = 0; y <= gridCellsY; y++)
                {
                    DrawLine(0, y * tileSize, worldWidth, y * tileSize, LIGHTGRAY);
                }

                // Center marker
                DrawRectangle(worldWidth / 2 - 25, worldHeight / 2 - 25, 50, 50, RED);

            EndMode2D();

            // --- UI (Screen Space) ---
            DrawRectangle(10, 10, 340, 120, Fade(BLACK, 0.7f));
            DrawText("Hold Left Click & Drag to Pan", 20, 20, 18, RAYWHITE);
            DrawText("Use Mouse Wheel to Zoom", 20, 50, 18, RAYWHITE);
            DrawText("Press 'R' to Reset Camera", 20, 80, 18, RAYWHITE);
            DrawText(TextFormat("Current Zoom: %.2fx", camera.zoom), 20, 110, 18, YELLOW);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}