/*
        ------instruction-------------
  --------line 122-126 for variable ordering-----------
   ---------val ordering--------------
----137~139 FAV, or 141~146 random selection val ordering----
*/
#include<bits/stdc++.h>
using namespace std;

class Position
{
    public:
           int row, col;
};

class Cell{
    public:
          int domainSize;
          int remainingValue;
          bool hasConstraintForSmall=false;
          bool hasConstraintForGreater=false;
          int currentValue;
          bool alter=true;
          vector<int> domain;
          vector<Position> constraintForGreaterVal, constraintForSmallVal;
          Cell (int currentValue, int domainSize)
          {
              this->currentValue=currentValue;
              this->domainSize=domainSize;
              if(currentValue!=0)
              {
                  alter=false;
                  remainingValue=0;
              }
              else
              {
                  this->remainingValue=domainSize;
                  domain[domainSize];
                  //domain=new int[this.domainSize];
                  for(int i=0;i<this->domainSize;i++)
                  {
                      domain.push_back(i+1);
                  }
              }
          }

          bool isAvailable()
          {
              return alter;
          }
          void setConstraintRCForGreaterThanSquare(int constraintRow, int constraintCol)
          {
              Position pos=Position();
              pos.row=constraintRow;
              pos.col=constraintCol;
              ///need to check
              constraintForGreaterVal.push_back(pos);
              this->hasConstraintForGreater=true;
          }

           void setConstraintRCForLessThanSquare(int constraintRow, int constraintCol)
          {
              Position pos=Position();
              pos.row=constraintRow;
              pos.col=constraintCol;
              ///need to check
              constraintForSmallVal.push_back(pos);
              this->hasConstraintForSmall=true;
          }

          void setCurrentValue(int currentValue)
          {
              this->currentValue=currentValue;
          }

          void decreaseRemainingValue()
          {
              this->remainingValue--;
          }
          void increaseRemainingValue()
          {
              this->remainingValue++;
          }
};

class Board{
public:
  //  Cell  grids[9][9] ;
    int N, domain[],nodeCount;
    vector<vector<Cell> > grids;
    Board(vector<vector<Cell> > grids, int N)
    {
        this->grids=grids;
        this->N=N;
        nodeCount=0;
    }

    int getN()
    {
        return N;
    }
    bool isFinished()
    {
        for(int i=0;i<N;i++)
        {
            for(int j=0;j<N;j++)
            {
                if(grids[i][j].currentValue==0)
                {
                    return false;
                }
            }
        }
        return true;
    }

    bool recursiveBacktracking()
    {
        showBoard(); //
        if(isFinished())
        {
            return true;
        }

    ///var ordering
    Position pos=MinimumRemainingValue();
    //Position pos=RandomSelection();
 //   Position pos=Degree();
 //   Position pos=FirstAvailable();

    int r,c,val;
    r=pos.row;
    c=pos.col;
    val=-9999;
    for(int i=0;i<N;i++)
    {
        ///val ordering////////////////////////////////////
        ///137~139 FAV, or 141~146 random selection val ordering///
        //first available val ordering
        if(grids[r][c].domain[i]!=-1)
        {
            val=grids[r][c].domain[i];

        //randomSelection
//        int p=rand()%N;
//        if(grids[r][c].domain[p]!=-1)
//        {
//            val=grids[r][c].domain[p];
//            i=p;
            if(checkConstraint(pos,val))
            {
                grids[r][c].setCurrentValue(val);
                grids[r][c].domain[i]=-1;
                grids[r][c].decreaseRemainingValue();
                nodeCount++;
                checkRowColAvailability(r,c,i);
                bool result=recursiveBacktracking();
                if(result!=false)
                    return result;
                revertChange(r,c,i);

            }


        }
    }
    return false;
  }

   void checkRowColAvailability(int r, int c, int i)
   {
       for(int k=0;k<N;k++)
       {
           if(k!=r && grids[k][c].isAvailable())
           {
               grids[k][c].domain[i]=-1;
               if((grids[k][c].remainingValue)>0)
               {
                   grids[k][c].decreaseRemainingValue();
               }
           }

           if(k!=c && grids[r][k].isAvailable())
           {
               grids[r][k].domain[i]=-1;
               if((grids[r][k].remainingValue)>0)
               {
                   grids[r][k].decreaseRemainingValue();
               }
           }

       }
   }

   void revertChange(int r, int c, int i)
   {
       grids[r][c].setCurrentValue(0);
       grids[r][c].increaseRemainingValue();
       grids[r][c].domain[i]=i+1;
       for(int k=0;k<N;k++)
       {
           if(k!=r && grids[k][c].isAvailable())
           {
               grids[k][c].domain[i]=i+1;
               if((grids[k][c].remainingValue) <N)
               {
                   grids[k][c].increaseRemainingValue();
               }
           }

            if(k!=c && grids[r][k].isAvailable())
           {
               grids[r][k].domain[i]=i+1;
               if((grids[r][k].remainingValue) <N)
               {
                   grids[r][k].increaseRemainingValue();
               }
           }
       }

   }

   Position MinimumRemainingValue()
   {
       int minVal= 9999;
       Position position=Position();
       for(int i=0;i<N;i++)
       {
           for(int j=0;j<N;j++)
           {
               if(grids[i][j].isAvailable()&& grids[i][j].remainingValue < minVal && grids[i][j].currentValue==0)
               {
                   minVal=grids[i][j].remainingValue;
                   position.row=i;
                   position.col=j;
               }
           }
       }
       return position;
   }

  Position RandomSelection()
  {
      Position position= Position();
       for(int i=0;i<N;i++)
       {
           for(int j=0;j<N;j++)
           {
               int r,c;
               r=rand()%N;
               c=rand()%N;
               if(grids[r][c].isAvailable() && grids[r][c].currentValue==0)
               {
                   position.row=r;
                   position.col=c;
                   return position;
               }
           }
       }
       return position;
  }

  Position Degree()
  {
    int max=-9999;
    Position position=Position();
    for(int i=0;i<N;i++)
    {
        for(int j=0;j<N;j++)
        {
            if(grids[i][j].isAvailable() && grids[i][j].currentValue==0)
            {
                int totalConstraint=grids[i][j].constraintForGreaterVal.size()+
                        grids[i][j].constraintForSmallVal.size();
                        if(totalConstraint > max)
                        {
                            max=totalConstraint;
                            position.row=i;
                            position.col=j;
                        }
            }
        }
    }
    return position;
  }

  Position FirstAvailable()
  {
      Position position=Position();
   //   cout<<" fdsrfdsf"<<endl;
      for(int i=0;i<N;i++)
      {
          for(int j=0;j<N;j++)
          {
              if(grids[i][j].isAvailable() && grids[i][j].currentValue==0)
              {
                  position.row=i;
                  position.col=j;
              }
          }
      }
      return position;
  }

   bool checkConstraint(Position pos, int value)
  {

      bool constraintStatusSmall=grids[pos.row][pos.col].hasConstraintForSmall;

      if(constraintStatusSmall==true)
      {
         vector<Position>* constraintList; ///have to check
         constraintList = &grids[pos.row][pos.col].constraintForSmallVal; ///need to recheck
         for(int i=0;i<constraintList->size();i++)
         {
              int constraintRow, constraintCol;
              ///need to recheck...
              constraintRow=(*constraintList)[i].row;
              constraintCol=(*constraintList)[i].col;
              if(value <=grids[constraintRow][constraintCol].currentValue && grids[constraintRow][constraintCol].currentValue!=0)
              {
                  return false;
              }

         }

      }


      bool constraintStatusGreater=grids[pos.row][pos.col].hasConstraintForGreater;
      if(constraintStatusGreater==true)
      {
            vector<Position>* constraintList; ///have to check
            constraintList = &grids[pos.row][pos.col].constraintForGreaterVal; ///need to recheck

            for(int i=0;i<constraintList->size();i++)
            {
              int constraintRow, constraintCol;
              ///need to recheck...
              constraintRow=(*constraintList)[i].row;
              constraintCol=(*constraintList)[i].col;
            if(value >=grids[constraintRow][constraintCol].currentValue && grids[constraintRow][constraintCol].currentValue!=0)
                {
                    return false;
               }
            }

      }

      ///row wise check

      for(int i=0; i<this->N; i++)
        {
        if(i!=pos.row)
              {
                if(grids[i][pos.col].currentValue==value)
                  {
                    return false;
                  }
              }
       }

      ///col wise checking

      for(int i=0; i<this->N; i++)
        {
            if(i!=pos.col)
            {
                if(grids[pos.row][i].currentValue==value)
                {
                    return false;
                }
            }
        }

        return true;
    }


    void simulate()
    {
        recursiveBacktracking();
        cout<<"total node count: "<<nodeCount<<endl;
    }

    void showBoard()
    {
        cout<<"---------Puzzle State-------"<<endl;
        for(int i=0;i<N;i++)
        {
            for(int j=0;j<N;j++)
            {
                cout<<grids[i][j].currentValue<<" ";
            }
            cout<<endl;
        }
    }
};



int main()
{
     int a,b,c,d,e,f,g,h,x,y,z,t,N,M;
     srand(time(0));

     clock_t t1,t2;

     vector<vector<Cell> > grids;

    fstream infile("in1.txt");
    cout << "in1.txt opened" << endl;
//    fstream infile("in1.txt");
//    fstream infile("in2.txt");
//    fstream infile("in3.txt");
    //fstream infile("in7-1.txt");


    while (infile >>a)
    {
       N=a;
       cout<<N<<endl;
       break;
    }
     if(N==5)
     {
        int i=0;
        while(i<N)
        {
        	infile>>a >> b >> c >> d >> e;
        	vector<Cell> gridValue;
        	gridValue.push_back(*new Cell(a,5));
        	gridValue.push_back(*new Cell(b,5));
        	gridValue.push_back(*new Cell(c,5));
        	gridValue.push_back(*new Cell(d,5));
        	gridValue.push_back(*new Cell(e,5));
        	grids.push_back(gridValue);
            /*grids[i][0].currentValue=a;
            grids[i][1].currentValue=b;
            grids[i][2].currentValue=c;
            grids[i][3].currentValue=d;
            grids[i][4].currentValue=e;*/
            i++;
        }

     }
     else if(N==7)
     {
         int i=0;
         while(i<N)
        {
        	infile>>a >> b >> c >> d >> e >> f >> g;
        	vector<Cell> gridValue;
        	gridValue.push_back(*new Cell(a,7));
        	gridValue.push_back(*new Cell(b,7));
        	gridValue.push_back(*new Cell(c,7));
        	gridValue.push_back(*new Cell(d,7));
        	gridValue.push_back(*new Cell(e,7));
        	gridValue.push_back(*new Cell(f,7));
        	gridValue.push_back(*new Cell(g,7));
        	grids.push_back(gridValue);
            /*grids[i][0].currentValue=a;
            grids[i][1].currentValue=b;
            grids[i][2].currentValue=c;
            grids[i][3].currentValue=d;
            grids[i][4].currentValue=e;
            grids[i][5].currentValue=f;
            grids[i][6].currentValue=g;*/
            i++;
        }
     }
	 infile.close();
    ///testing purpose
//     for(int i=0;i<N;i++)
//     {
//         for(int j=0;j<N;j++)
//         {
//             cout<<grids[i][j].currentValue<<" ";
//         }
//         cout<<endl;
//     }

       ///constraint
  //  fstream inf("const1.txt");
    cout << "const1.txt opened" << endl;
//    fstream inf("const2.txt");
//    fstream inf("const3.txt");
    fstream inf("const7-1.txt");

    while (inf>>a)
    {
       M=a;
       cout<<M<<endl;
       break;
    }
    int s=1;
    while(inf>>a >> b >> c >> d )
    {
        grids[a][b].setConstraintRCForLessThanSquare(c,d);
        grids[c][d].setConstraintRCForGreaterThanSquare(a,b);

    }
    inf.close();
    Board board= Board(grids,N);
    t1=clock();
    board.simulate();
    t2=clock();
    double time=float(float(t2)-float(t1))/1000;
    cout<<"Total execution time: "<< time<<endl;



    return 0;

}
