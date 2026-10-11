#ifndef GRID_HPP
#define GRID_HPP
#include <vector> 
class grid
{
    private:
        int n;
        std::vector<int> Ovec;
        std::vector<int> Xvec;
    public: 
        grid (int n_, std::vector<int> Ovec_, std::vector<int> Xvec_);
        std::vector<int>& getVec(char symbol);
        std::vector<int>& getDualVec(char symbol);
        int getn();
        void setn(int _n);
};
#endif
