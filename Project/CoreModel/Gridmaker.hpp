#ifndef GRIDMAKER_HPP
#define GRIDMAKER_HPP

#include "grid.hpp"
#include "raylib.h"

int pickmove (int commu, int stab);
class Gridmaker
{
    private: 
        grid currentgrid;
    public:
        Gridmaker ();
        grid grid_generator (int n);
        std::vector<bool> check_destabilizable_subgrid(int randompos, auto& symbolvec, auto& dualsymbolvec, int rowdiff, int coldiff, int n);
        void do_commutation_move(auto&,auto&,int,bool,Vector2);
        void do_destab_move(std::vector<bool>,auto&,auto&,int,int*,int*,int,grid&);
        void do_cromwell_move (int crom, grid& currentgrid);
};

#endif