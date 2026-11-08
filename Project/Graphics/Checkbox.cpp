#include "Checkbox.hpp"
#include <raylib.h>
#include <iostream>
#include <string>

Checkbox::Checkbox(Rectangle Rectbox_)
{
    Rectbox = Rectbox_;
    letterCount = 0;       
    text[0] = '\0';
    MouseOnText = false;
}

int Checkbox::get_size_from_text() {return std::stoi(text);}

void Checkbox::Draw()
{
    DrawRectangleLinesEx(Rectbox,3,WHITE);
    if (CheckCollisionPointRec(GetMousePosition(), Rectbox))
        DrawRectangleLinesEx(Rectbox, 3, {255,0,255,255});
    else
        DrawRectangleLinesEx(Rectbox, 3, WHITE);

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        if (CheckCollisionPointRec(GetMousePosition(), Rectbox))
        {
            DrawRectangleLinesEx(Rectbox, 3, WHITE);
            DrawText("|", (int)Rectbox.x + 175, (int)Rectbox.y + 10, 120, WHITE);
        }
    }
    int textWidth = MeasureText(text, 120);
    int textX = (int)(Rectbox.x + (Rectbox.width - textWidth) / 2);
    int textY = (int)(Rectbox.y + (Rectbox.height - 120) / 2);
    DrawText(text, textX, textY, 120, WHITE);
}		

void Checkbox::Handler()
{
    
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        if (CheckCollisionPointRec(GetMousePosition(), Rectbox))
            MouseOnText = true;
        else
            MouseOnText = false;
    }

    if (MouseOnText)
        {
            int key = GetCharPressed();
            while (key > 0)
            {
                if ((key >= 32) && (key <= 57) && (letterCount < 3))
                {
                    text[letterCount] = (char)key;
                    letterCount++;
                    text[letterCount] = '\0';
                }
                key = GetCharPressed();
                std::cout<<key<<std::endl;
            }
            if (IsKeyPressed(KEY_BACKSPACE))
            {
                letterCount--;
                if (letterCount < 0) letterCount = 0;
                text[letterCount] = '\0';
            }
        }

}

