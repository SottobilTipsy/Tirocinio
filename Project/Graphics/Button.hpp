#ifndef BUTTON_H
#define BUTTON_H

#include <raylib.h>
#include <string>

class Button
{
    private:
        Rectangle buttonrect;
        Vector2 position;
    public:
        Button(Rectangle,Vector2);
        void DrawAsFont(Font,const char*,Vector2,int,Color);
        bool IsPressed(bool);
};

#endif

