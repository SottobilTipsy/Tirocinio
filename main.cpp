#include <iostream>
#include <vector>
#include <raylib.h>
#include <algorithm>

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

class grid
{
    private:
        int n;
        std::vector<int> Ovec;
        std::vector<int> Xvec;

    public: 
        grid (int n_, std::vector<int> Ovec_, std::vector<int> Xvec_)
            : n(n_), Ovec(Ovec_), Xvec(Xvec_) {}

        std::vector<int>& getVec(char symbol) { return (symbol == 'O') ? Ovec : Xvec; }

        std::vector<int>& getDualVec(char symbol) { return (symbol == 'O') ? Xvec : Ovec; }

        int getn() { return n; }

        void setn(int _n) { n = _n; }

        static grid Gridmaker (int n)
        {
            std::vector<int> Ovec = {1,2};                  //step1
            std::vector<int> Xvec = {2,1};
            grid currentgrid = grid(2,Ovec,Xvec);

            for (int i=0 ; i < 50 ; i++)
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

                int crom = 0;                               // step3
                if(currentgrid.getn() == 2)    
                    crom = pickmove(0,100);
                else if (currentgrid.getn() > 2 && currentgrid.n < n)
                    crom = pickmove(45,45);
                else if (currentgrid.getn() == n)
                    crom = pickmove(80,20);
                else if (currentgrid.getn() > n)
                    crom = pickmove(45,10);

                currentgrid.docromwellmove(crom,currentgrid.getn());  //step 4
            }
            return currentgrid;
        }

        std::vector<bool> check_destabilizable_subgrid(int randompos, auto symbolvec, auto dualsymbolvec, int rowdiff, int coldiff)
        {
            std::vector<bool> verify = {false,false};
            if (symbolvec[randompos+rowdiff] == symbolvec[randompos]+coldiff)
            {
                verify[0]= true;
                if (dualsymbolvec[randompos] == symbolvec[randompos]+coldiff) 
                    verify[1] = false;
                else
                    verify[1]= true;
            }
            return verify;
        }

        void docromwellmove (int crom, int n)
        {
            int randompos = 0;
            char chosensymb;
            std::vector<bool> destabilizable = {false,false};
            std::vector<int> rowdiff = {-1,-1,1,1};
            std::vector<int> coldiff = {-1,1,-1,1};
            int whichcorner = 0;
            if (crom == -1)
            {
                int *seq = LoadRandomSequence(n*2,0,(n*2)-1);
                for (int i = 0 ; i<n ; i++)
                {
                    if (seq[i] < n )
                    {
                        randompos = seq[i];
                        chosensymb = 'X';
                    }
                    else
                    {
                        randompos = seq[i]-n;
                        chosensymb = 'O';
                    }
                    
                    auto& symbolvec = getVec(chosensymb);
                    auto& dualsymbolvec = getDualVec(chosensymb);
                    while(destabilizable[0] == false)
                    {
                        destabilizable = check_destabilizable_subgrid(randompos,symbolvec,dualsymbolvec,rowdiff[whichcorner],coldiff[whichcorner]);
                        whichcorner++;
                    }
                }
                UnloadRandomSequence(seq);
            }
            else
            {
                randompos = GetRandomValue(0,n-1); 
                int whichsymb = GetRandomValue(0,1);
                if (whichsymb == 0)
                    chosensymb = 'X';
                else
                    chosensymb = 'O';
            }

            std::cout<<"chosen symbol is "<<chosensymb<<" row "<<randompos<<" move "<<crom<<std::endl;

            auto& symbolvec = getVec(chosensymb);
            auto& dualsymbolvec = getDualVec(chosensymb);

            if (crom == -1)
            {
                if (destabilizable[0] == false)
                {
                    std::cout<<"not destabilizable";
                    return;
                }

                std::cout<<"destabilize\n";
                int deletedcolumn = symbolvec[randompos]-coldiff[whichcorner];
                int newdualsymbol = dualsymbolvec[randompos+rowdiff[whichcorner]];
                dualsymbolvec[randompos+rowdiff[whichcorner]] = newdualsymbol;
                symbolvec.erase(symbolvec.begin()+randompos+rowdiff[whichcorner]);
                dualsymbolvec.erase(dualsymbolvec.begin()+randompos+rowdiff[whichcorner]);
                
                for (int& val : symbolvec) 
                    if (val > deletedcolumn) val--;
                for (int& val : dualsymbolvec) 
                    if (val > deletedcolumn) val--;

                setn(getn() - 1);
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
                        std::cout<<"commute bottom row"<<std::endl;
                    }
                    else 
                    {
                        if(randompos == 0)
                            return;
                        rowdifference = -1;
                        std::cout<<"commute top row"<<std::endl;
                    }
                        
                    int B1 = symbolvec[randompos+rowdifference];
                    int B2 = dualsymbolvec[randompos+rowdifference];
                    if (B1 > B2) 
                        std::swap(B1,B2);
                    
                    if (A1 < B1 && B1 < A2 && A2 < B2)
                        return;
                    std::cout<<"success"<<std::endl;
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
                        std::cout<<"commute left column"<<std::endl;
                    }
                    else 
                    {
                        if (symbolvec[randompos] == symbolvec.size())
                            return;
                        columndifference = 1;
                        std::cout<<"commute right column"<<std::endl;
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
                    std::cout<<"success"<<std::endl;
                    std::swap(symbolvec[randompos],symbolvec[symbcolumn]);
                    std::swap(dualsymbolvec[dualsymbcolumn],dualsymbolvec[dualsymbcolumn2]);
                }
            }

            else if (crom == 1)
            {
                setn(getn()+1);
                int whichemptycorner = GetRandomValue(1,4);
                int value = symbolvec[randompos];

                for (int& val : symbolvec) 
                    if (val > value) val++;
                for (int& val : dualsymbolvec) 
                    if (val > value) val++;
    
                if (whichemptycorner == 1)
                {
                    std::cout<<"stabilize with top left empty"<<std::endl;
                    symbolvec.insert(symbolvec.begin()+randompos+1,value);
                    symbolvec[randompos]= value+1;
                    dualsymbolvec.insert(dualsymbolvec.begin()+randompos+1,value+1);
                }
                else if (whichemptycorner == 2)
                {
                    std::cout<<"stabilize with top right empty"<<std::endl;
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
                    std::cout<<"stabilize with bottom left empty"<<std::endl;
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
                    std::cout<<"stabilize with bottom right empty"<<std::endl;
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
                std::cout<<"success"<<std::endl;
            }
            
        }
};

int main()
{
    SetRandomSeed(time(NULL));
    grid finalgrid = grid::Gridmaker(500);
    std::cout << "ciao\n";
}