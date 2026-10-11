#include <iostream>
#include "Gridmaker.hpp"

int main()
{
    Gridmaker Gridmaker;
    grid finalgrid = Gridmaker.grid_generator(50);
    for (int i = 0 ; i < finalgrid.getn() ; i++)
        std::cout<<finalgrid.getVec('X')[i]<<" ";
    std::cout<<std::endl;
    for (int i = 0 ; i < finalgrid.getn() ; i++)
        std::cout<<finalgrid.getDualVec('X')[i]<<" ";
    std::cout<<std::endl;
    std::cout << "ciao\n";
}