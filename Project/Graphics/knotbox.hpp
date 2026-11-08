#ifndef KNOTBOX_HPP
#define KNOTBOX_HPP

#include "grid.hpp"
#include "raylib.h"

class Knotbox
{
    private:
        grid knotgrid;
        int cellsize;
        Rectangle HighlightColumnrect;
        bool clicked_once;
        bool swapped;
        int previous_cell_info[2];

    public:
        Knotbox();
        Knotbox(grid);
        bool Calculate_Cell_Info(Vector2&,int&,int&,Camera2D);
        void Handler(Camera2D);
        void Draw();
};

#endif