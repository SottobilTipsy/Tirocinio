#include <iostream>
#include "Gridmaker.hpp"
#include "temp.h"

int main()
{
    Gridmaker Gridmaker;
    grid finalgrid = Gridmaker.grid_generator(25);
    for (int i = 0 ; i < finalgrid.getn() ; i++)
        std::cout<<finalgrid.getVec('X')[i]<<" ";
    std::cout<<std::endl;
    for (int i = 0 ; i < finalgrid.getn() ; i++)
        std::cout<<finalgrid.getDualVec('X')[i]<<" ";
    std::cout<<std::endl;
    std::cout << "ciao\n";
    tempgame();
}