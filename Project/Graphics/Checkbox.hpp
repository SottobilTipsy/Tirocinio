#ifndef CHECKBOX_HPP
#define CHECKBOX_HPP

#include <raylib.h>
#include <vector>

class Checkbox
{
    private:
        Rectangle Rectbox;
        char text[4];
        int letterCount; 
        bool MouseOnText;

    public:
        Checkbox(Rectangle);
        int get_size_from_text();
        void Handler();
        void Draw();
};

#endif