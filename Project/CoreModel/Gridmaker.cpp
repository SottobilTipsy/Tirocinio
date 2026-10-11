#include <iostream>
#include <vector>
#include <raylib.h>
#include <algorithm>
#include <numeric>
#include <random>   

#include "grid.hpp"
#include "Gridmaker.hpp"

int pickmove (int commu, int stab)
{
    int move = 0;
    float randomvalue = GetRandomValue(1,100);
    if (randomvalue > 0 && randomvalue <= commu)
        ;
    else if (randomvalue > commu && randomvalue <= commu+stab)
        move = 1;
    else if (randomvalue > commu+stab && randomvalue <= 100)
        move = -1;
    return move;
}

Gridmaker::Gridmaker()
    : currentgrid(2, {1, 2}, {2, 1}) {}

grid Gridmaker::grid_generator (int n)
{
    SetRandomSeed(time(NULL));
    for (int i=0 ; i < n*10 ; i++)
    {
        
        for (int i = 1 ; i <= currentgrid.getn() ; i++)
        {
            for (int j = 1 ; j <= currentgrid.getn() ; j++)
            {
                if (currentgrid.getVec('X')[i-1] == j)
                    std::cout<<"X ";
                else if (currentgrid.getDualVec('X')[i-1] == j)
                    std::cout<<"O ";
                else
                    std::cout<<"  ";    
            }
            std::cout<<"\n";

        }
        std::cout << "loop" << i << std::endl; 

        int crom = 0;                               
        if(currentgrid.getn() == 2)    
            crom = pickmove(0,100);
        else if (currentgrid.getn() > 2 && currentgrid.getn() < n)
            crom = pickmove(50,40);
        else if (currentgrid.getn() == n)
            crom = pickmove(80,20);
        else if (currentgrid.getn() > n)
            crom = pickmove(50,30);

        do_cromwell_move(crom,currentgrid);  
    }
    std::cout<<"entering while loop";
    while (currentgrid.getn() != n)
    {
        if ( currentgrid.getn() < n )
            do_cromwell_move(1,currentgrid); 
        else
        {
            int previous_n = currentgrid.getn();
            do_cromwell_move(-1,currentgrid); 
            if (currentgrid.getn() == previous_n)
                do_cromwell_move(0,currentgrid);
        }
        
    }
    return currentgrid;
}

std::vector<bool> Gridmaker::check_destabilizable_subgrid(int randompos, auto& symbolvec, auto& dualsymbolvec, int rowdiff, int coldiff, int n)
{
    std::vector<bool> verify = {false,false};
    if (randompos + rowdiff < 0 || randompos + rowdiff >= n)
        return verify;
    if (symbolvec[randompos] + coldiff < 1 || symbolvec[randompos] + coldiff > n )
        return verify;

    if (symbolvec[randompos+rowdiff] == symbolvec[randompos]+coldiff)
    {
        if (dualsymbolvec[randompos] == symbolvec[randompos]+coldiff) 
            { verify[0] = true; verify[1] = false; }
        else if (dualsymbolvec[randompos+rowdiff] == symbolvec[randompos])
            { verify[0] = true; verify[1] = true; }
    }
    return verify;
}

void Gridmaker::do_cromwell_move (int crom, grid& currentgrid)
{
    int randompos = 0;
    char chosensymb;
    std::vector<bool> destabilizable = {false,false};
    int rowdiff[4] = {-1,-1,1,1};
    int coldiff[4] = {-1,1,-1,1};
    int whichcorner;
    //std::cout<<crom<<std::endl;
    if (crom == -1)
    {
        std::vector<int> seq(currentgrid.getn()*2);
        std::iota(seq.begin(), seq.end(), 0);
        static std::mt19937 g(std::random_device{}());
        std::shuffle(seq.begin(), seq.end(), g);
        for (int i = 0 ; i<currentgrid.getn()*2 ; i++)
        {
            if (destabilizable[0] == true)
                    break;
            whichcorner = 0;
            if (seq[i] < currentgrid.getn())
            {
                randompos = seq[i];
                chosensymb = 'X';
            }
            else
            {
                randompos = seq[i]-currentgrid.getn();
                chosensymb = 'O';
            }
            
            auto& symbolvec = currentgrid.getVec(chosensymb);
            auto& dualsymbolvec = currentgrid.getDualVec(chosensymb);
            while(destabilizable[0] == false && whichcorner < 4)
            {
                destabilizable = check_destabilizable_subgrid(randompos,symbolvec,dualsymbolvec,rowdiff[whichcorner],coldiff[whichcorner],currentgrid.getn());
                if (destabilizable[0] == true)
                    break;
                whichcorner++;
            }
        }
    }
    else
    {
        randompos = GetRandomValue(0,currentgrid.getn()-1); 
        int whichsymb = GetRandomValue(0,1);
        if (whichsymb == 0)
            chosensymb = 'X';
        else
            chosensymb = 'O';
    }

    // std::cout<<"chosen symbol is "<<chosensymb<<" row "<<randompos<<" move "<<crom<<std::endl;

    auto& symbolvec = currentgrid.getVec(chosensymb);
    auto& dualsymbolvec = currentgrid.getDualVec(chosensymb);

    if (crom == -1)
    {
        if (destabilizable[0] == false)
            return;

        //std::cout<<"destabilize\n";
        int deletedcolumn = symbolvec[randompos]+coldiff[whichcorner];
        int newdualsymbol = dualsymbolvec[randompos+rowdiff[whichcorner]];
        if (destabilizable[1] == false)
        {
            int newdualsymbol = dualsymbolvec[randompos+rowdiff[whichcorner]];
            dualsymbolvec[randompos] = newdualsymbol;
        }
        else
        {
            auto it = std::find(dualsymbolvec.begin(),dualsymbolvec.end(),symbolvec[randompos]+coldiff[whichcorner]);
            int newdualsymbolrow = std::distance(dualsymbolvec.begin(), it);
            dualsymbolvec[newdualsymbolrow] = symbolvec[randompos];
        }
        symbolvec.erase(symbolvec.begin()+randompos+rowdiff[whichcorner]);
        dualsymbolvec.erase(dualsymbolvec.begin()+randompos+rowdiff[whichcorner]);
        
        
        for (int& val : symbolvec) 
            if (val > deletedcolumn) val--;
        for (int& val : dualsymbolvec) 
            if (val > deletedcolumn) val--;

        currentgrid.setn(currentgrid.getn() - 1);
    }

    else if (crom == 0)
    {
        int commuteroworcolumn = GetRandomValue(0,1);
        if(commuteroworcolumn == 0)
        {
            int A1 = symbolvec[randompos];
            int A2 = dualsymbolvec[randompos];
            if (A1 > A2) 
                std::swap(A1,A2);

            int bottomortoprow = GetRandomValue(0,1);
            int rowdifference = 0;
            if(bottomortoprow == 0)
            {
                if (randompos >= symbolvec.size() - 1)
                    return;
                rowdifference = 1;
                //std::cout<<"commute bottom row"<<std::endl;
            }
            else 
            {
                if(randompos == 0)
                    return;
                rowdifference = -1;
                //std::cout<<"commute top row"<<std::endl;
            }
                
            int B1 = symbolvec[randompos+rowdifference];
            int B2 = dualsymbolvec[randompos+rowdifference];
            if (B1 > B2) 
                std::swap(B1,B2);
            
            if (A1 < B1 && B1 < A2 && A2 < B2)
                return;
            //std::cout<<"success"<<std::endl;
            std::swap(symbolvec[randompos],symbolvec[randompos+rowdifference]);
            std::swap(dualsymbolvec[randompos],dualsymbolvec[randompos+rowdifference]);
        }
        else 
        {
            int A1 = randompos;
            auto it = std::find(dualsymbolvec.begin(),dualsymbolvec.end(),symbolvec[A1]);
            int dualsymbcolumn = it - dualsymbolvec.begin();
            int A2 = dualsymbcolumn;
            if (A1 > A2) 
                std::swap(A1,A2);

            int leftorrightcolumn = GetRandomValue(0,1);
            int columndifference = 0;
            if(leftorrightcolumn == 0)
            {
                if (symbolvec[randompos] == 1)
                    return;
                columndifference = -1;
                //std::cout<<"commute left column"<<std::endl;
            }
            else 
            {
                if (symbolvec[randompos] == symbolvec.size())
                    return;
                columndifference = 1;
                //std::cout<<"commute right column"<<std::endl;
            }

            it = std::find(symbolvec.begin(),symbolvec.end(),symbolvec[randompos]+columndifference);
            int symbcolumn = it - symbolvec.begin();
            int B1 = symbcolumn; 
            it = std::find(dualsymbolvec.begin(),dualsymbolvec.end(),symbolvec[randompos]+columndifference);
            int dualsymbcolumn2 = it - dualsymbolvec.begin();
            int B2 = dualsymbcolumn2;
            if (B1 > B2) 
                std::swap(B1,B2);
            
            if (A1 < B1 && B1 < A2 && A2 < B2)
                return;
            //std::cout<<"success"<<std::endl;
            std::swap(symbolvec[randompos],symbolvec[symbcolumn]);
            std::swap(dualsymbolvec[dualsymbcolumn],dualsymbolvec[dualsymbcolumn2]);
        }
    }

    else if (crom == 1)
    {
        currentgrid.setn(currentgrid.getn()+1);
        int whichemptycorner = GetRandomValue(1,4);
        int value = symbolvec[randompos];

        for (int& val : symbolvec) 
            if (val > value) val++;
        for (int& val : dualsymbolvec) 
            if (val > value) val++;

        if (whichemptycorner == 1)
        {
            //std::cout<<"stabilize with top left empty"<<std::endl;
            symbolvec.insert(symbolvec.begin()+randompos+1,value);
            symbolvec[randompos]= value+1;
            dualsymbolvec.insert(dualsymbolvec.begin()+randompos+1,value+1);
        }
        else if (whichemptycorner == 2)
        {
            //std::cout<<"stabilize with top right empty"<<std::endl;
            symbolvec.insert(symbolvec.begin()+randompos+1,value+1);
            auto it = std::find(dualsymbolvec.begin(),dualsymbolvec.end(),value);
            if (it == dualsymbolvec.end()) 
                return;
            int dualrow = std::distance(dualsymbolvec.begin(), it);
            dualsymbolvec[dualrow]=value+1;
            dualsymbolvec.insert(dualsymbolvec.begin()+randompos+1,value);  
        }
        else if (whichemptycorner == 3)
        {
            //std::cout<<"stabilize with bottom left empty"<<std::endl;
            auto it = std::find(dualsymbolvec.begin(),dualsymbolvec.end(),value);
            if (it == dualsymbolvec.end()) 
                return;
            int dualrow = std::distance(dualsymbolvec.begin(), it);
            int originaldualval = dualsymbolvec[dualrow];
            symbolvec.insert(symbolvec.begin()+randompos+1,value+1);
            dualsymbolvec.insert(dualsymbolvec.begin()+randompos+1,originaldualval);
            if (dualrow >= randompos+1) 
                dualrow++;
            dualsymbolvec[dualrow] = value+1;
        }
        else if (whichemptycorner == 4)
        {
            //std::cout<<"stabilize with bottom right empty"<<std::endl;
            auto it = std::find(dualsymbolvec.begin(),dualsymbolvec.end(),value);
            if (it == dualsymbolvec.end()) 
                return;
            int dualrow = std::distance(dualsymbolvec.begin(), it);
            symbolvec.insert(symbolvec.begin()+randompos+1,value);
            symbolvec[randompos] = value+1;
            dualsymbolvec[dualrow] = value+1;
            int dualcolumn = dualsymbolvec[randompos];
            dualsymbolvec[randompos] = value;
            dualsymbolvec.insert(dualsymbolvec.begin()+randompos+1,dualcolumn);
        }
        //std::cout<<"success"<<std::endl;
    }
    
}