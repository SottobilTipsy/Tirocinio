#include "grid.hpp"
#include <vector> 

grid::grid(int n_, std::vector<int> Ovec_, std::vector<int> Xvec_)
            : n(n_), Ovec(Ovec_), Xvec(Xvec_) {}

std::vector<int>& grid::getVec(char symbol) 
{ 
    return (symbol == 'O') ? Ovec : Xvec; 
}

std::vector<int>& grid::getDualVec(char symbol) 
{ 
    return (symbol == 'O') ? Xvec : Ovec; 
}

int grid::getn() 
{ 
    return n; 
}
void grid::setn(int _n) 
{ 
    n = _n;
}
