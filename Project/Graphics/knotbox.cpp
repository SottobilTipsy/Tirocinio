#include "knotbox.hpp"
#include "grid.hpp"
#include "Gridmaker.hpp"
#include <raylib.h>
#include <algorithm>
#include <iterator>
#include <iostream>

Color cellcolor = {200,200,200,255};
Color Hor_line_color = {106,13,173,255};

Knotbox::Knotbox()
{}

Knotbox::Knotbox(grid finalgrid)
   : knotgrid(finalgrid), cellsize(20), HighlightColumnrect({0,0,0,0}), clicked_once(false), swapped(false) {}

bool Knotbox::Calculate_Cell_Info(Vector2& cell_coordinates, int& cell_row, int& cell_column, 
    Camera2D cam)
{
    cell_coordinates = GetScreenToWorld2D(GetMousePosition(),cam);
    cell_row = (int)cell_coordinates.y/20;
    if ((int)cell_coordinates.y % 20 == 0 && cell_row != 0)
        cell_row--;
    cell_column = (int)cell_coordinates.x/20+1;
    if ((int)cell_coordinates.x % 20 == 0 && cell_column != 0)
        cell_column--;

    return true;
}

void Knotbox::Handler(Camera2D cam)
{
    Gridmaker maker;
    Vector2 cell_coordinates = {0,0};
    int cell_row = 0;
    int cell_column = 0;
    bool Mousepressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    if (Mousepressed)
    {
        for (int i = 1 ; i <= knotgrid.getn() ; i++)
        {
            for (int j = 1 ; j <= knotgrid.getn() ; j++)
            {
                if (knotgrid.getVec('X')[i-1] == j)
                    std::cout<<"X ";
                else if (knotgrid.getDualVec('X')[i-1] == j)
                    std::cout<<"O ";
                else
                    std::cout<<"  ";    
            }
            std::cout<<"\n";
        }

        Calculate_Cell_Info(cell_coordinates,cell_row,cell_column,cam);

        if (knotgrid.getVec('X')[cell_row] != cell_column && knotgrid.getVec('O')[cell_row] != cell_column)
            return;

        std::vector<int>& symbolvec = (knotgrid.getVec('X')[cell_row] == cell_column) 
            ? knotgrid.getVec('X') 
            : knotgrid.getVec('O');

        std::vector<int>& dualsymbolvec = (knotgrid.getVec('X')[cell_row] == cell_column) 
            ? knotgrid.getVec('O') 
            : knotgrid.getVec('X');

        int whichcorner = 0;
        if ((dualsymbolvec)[cell_row] == cell_column-1)
        {
            if ((dualsymbolvec)[cell_row-1] == cell_column)
                whichcorner = 1;
            else if ((dualsymbolvec)[cell_row+1] == cell_column)
                whichcorner = 3;
        }
        else if ((dualsymbolvec)[cell_row] == cell_column+1)
        {
            if ((dualsymbolvec)[cell_row-1] == cell_column)
                whichcorner = 2;
            else if ((dualsymbolvec)[cell_row+1] == cell_column)
                whichcorner = 4;
        }

        if (whichcorner != 0)
        {
            whichcorner = whichcorner--;
            int rowdiff[4] = {-1,-1,1,1};
            int coldiff[4] = {-1,1,-1,1};

            cell_column = cell_column + coldiff[whichcorner];
            if(symbolvec == knotgrid.getVec('X'))
                symbolvec = knotgrid.getVec('O');
            else
                symbolvec = knotgrid.getVec('X');
            if(dualsymbolvec == knotgrid.getVec('X'))
                dualsymbolvec = knotgrid.getVec('O');
            else
                dualsymbolvec = knotgrid.getVec('X');

            if(whichcorner == 0 || whichcorner == 2)
                whichcorner = whichcorner++;
            else if (whichcorner == 1 || whichcorner == 3)
                whichcorner = whichcorner--;

            std::cout<<cell_row<<std::endl;
            std::cout<<cell_column<<std::endl;
            std::cout<<symbolvec[cell_row]<<std::endl;
            std::cout<<dualsymbolvec[cell_row]<<std::endl;
            std::cout<<whichcorner<<std::endl;
            
            std::vector<bool> destabilizable = maker.check_destabilizable_subgrid(cell_row,symbolvec,dualsymbolvec,rowdiff[whichcorner],coldiff[whichcorner],knotgrid.getn());
            std::cout<<destabilizable[0]<<std::endl;

            maker.do_destab_move(destabilizable,symbolvec,dualsymbolvec,cell_row,rowdiff,coldiff,whichcorner,knotgrid);
        }
    }
    Mousepressed = IsMouseButtonPressed(MOUSE_BUTTON_RIGHT);
    if (Mousepressed)
    {
        Calculate_Cell_Info(cell_coordinates,cell_row,cell_column,cam);
    
        if(!clicked_once)
            HighlightColumnrect = {(cell_column-1)*cellsize,(cell_row)*cellsize,cellsize+1,cellsize+1};
        
        if (clicked_once)
        {
            std::vector<int>& symbolvec = knotgrid.getVec('X');
            std::vector<int>& dualsymbolvec= knotgrid.getDualVec('X');

            Vector2 directions = {2,2};
            if (cell_row == previous_cell_info[0]+1)
                { directions.x = 0; directions.y = 0; }
            else if (cell_row == previous_cell_info[0]-1)
                { directions.x = 0; directions.y = 1; }
            else if (cell_column == previous_cell_info[1]-1)
                { directions.x = 1; directions.y = 0; }
            else if (cell_column == previous_cell_info[1]+1)
                { directions.x = 1; directions.y = 1; }

            if (directions.x != 2 && directions.y != 2)
            {
                if(directions.x == 1)
                {
                    std::cout<<"ciao"<<std::endl;
                    auto it = std::find(symbolvec.begin(),symbolvec.end(),previous_cell_info[1]);
                    std::cout<<distance(symbolvec.begin(),it)<<std::endl;
                    previous_cell_info[0] = distance(symbolvec.begin(),it);
                }

                std::cout<<previous_cell_info[0]<<std::endl;
                std::cout<<"directions: "<<directions.x<<directions.y<<std::endl;
                maker.do_commutation_move(symbolvec,dualsymbolvec,previous_cell_info[0],false,directions);
                std::cout<<"MINCHIA\n";
                swapped = true;
            }
            else
            {
                swapped = true;
            }
        }

        clicked_once = true;
        if (swapped)
        { 
            clicked_once = false; 
            swapped = false;
            HighlightColumnrect = {0,0,0,0};
        }
        previous_cell_info[0] = cell_row;
        previous_cell_info[1] = cell_column;
    }

    bool EscPressed = IsKeyPressed(KEY_ESCAPE);
    if(EscPressed)
    {
        HighlightColumnrect = {0,0,0,0};
        clicked_once = false;
    }
}

void Knotbox::Draw()
{
    Vector2 startpos = {0,0};

    for (int row = 0 ; row < knotgrid.getn() ; row++)
    {
        for (int column = 0 ; column < knotgrid.getn() ; column++)
            DrawRectangle(startpos.x+column*cellsize+1,startpos.y+row*cellsize+1,cellsize-1,cellsize-1,cellcolor);

        int Xposition = knotgrid.getVec('X')[row]-1;
        int Oposition = knotgrid.getDualVec('X')[row]-1;

        // uncomment to show X and O points
        DrawRectangle(startpos.x+Xposition*cellsize+1,startpos.y+row*cellsize+1,cellsize-1,cellsize-1,BLUE);
        DrawRectangle(startpos.x+Oposition*cellsize+1,startpos.y+row*cellsize+1,cellsize-1,cellsize-1,RED);

        int hor_line_x_start = startpos.x+Xposition*cellsize+1+(cellsize/2);
        int hor_line_x_end = startpos.x+Oposition*cellsize+1+(cellsize/2);
        int hor_line_y_start = startpos.y+row*cellsize+1+(cellsize/2);
        int hor_line_y_end = startpos.y+row*cellsize+1+(cellsize/2);

        Vector2 start = {hor_line_x_start,hor_line_y_start};
        Vector2 end = {hor_line_x_end,hor_line_y_end};
        DrawLineEx(start,end,3,Hor_line_color);

        int ver_line_x_start = startpos.x+Xposition*cellsize+1+(cellsize/2);
        int ver_line_y_start = startpos.y+row*cellsize+1+(cellsize/2);
        auto it = std::find(knotgrid.getDualVec('X').begin(),knotgrid.getDualVec('X').end(),Xposition+1);
        int perpendiculardistance = std::distance(knotgrid.getDualVec('X').begin(), it);
        int ver_line_x_end = ver_line_x_start;
        int ver_line_y_end = startpos.y+perpendiculardistance*cellsize+1+(cellsize/2);

        start = {ver_line_x_start,ver_line_y_start};
        end = {ver_line_x_end,ver_line_y_end};
        DrawLineEx(start,end,3,Hor_line_color);

        ver_line_x_start = startpos.x+Oposition*cellsize+1+(cellsize/2);
        it = std::find(knotgrid.getVec('X').begin(),knotgrid.getVec('X').end(),Oposition+1);
        perpendiculardistance = std::distance(knotgrid.getVec('X').begin(), it);
        ver_line_x_end = ver_line_x_start;
        ver_line_y_end = startpos.y+perpendiculardistance*cellsize+1+(cellsize/2);

        start = {ver_line_x_start,ver_line_y_start};
        end = {ver_line_x_end,ver_line_y_end};
        DrawLineEx(start,end,3,Hor_line_color);

        DrawRectangleLinesEx(HighlightColumnrect,3,{255,0,255,255});
    }
}