#ifndef GRIDMAKER_HPP
#define GRIDMAKER_HPP

#include "grid.hpp"

int pickmove (int commu, int stab);
class Gridmaker
{
    private: 
        grid currentgrid;
    public:
        Gridmaker ();
        grid grid_generator (int n);
        std::vector<bool> check_destabilizable_subgrid(int randompos, auto& symbolvec, auto& dualsymbolvec, int rowdiff, int coldiff);
        void do_cromwell_move (int crom, grid& currentgrid);
};

#endif