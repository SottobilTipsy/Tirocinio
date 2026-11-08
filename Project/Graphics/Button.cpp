#include "Button.hpp"
#include <raylib.h>
#include <string>


Button::Button(Rectangle rect, Vector2 position_)
{
    buttonrect = rect;
    position = position_;
}

void Button::DrawAsFont(Font font, const char* text, Vector2 Pos, int fontsize, Color col)
{
     if (CheckCollisionPointRec(GetMousePosition(), buttonrect))
        DrawTextEx(font,text,Pos,fontsize,1,col);
    else
        DrawTextEx(font,text,{Pos.x+10,Pos.y+5},fontsize-5,1,col);
    return;
}

bool Button::IsPressed(bool MousePressed)
{
    if (CheckCollisionPointRec(GetMousePosition(), buttonrect) && MousePressed)
    {
        return true;
    }
    return false;
}