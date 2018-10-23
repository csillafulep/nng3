#include "header.h"
#include "global.h"
#include <cmath>

//calculate magic Score
double Board::calculateMagicScore()  const{
    const double row = double(Task::GetInstance().tableRowCount);
    const double col = double(Task::GetInstance().tableColCount);
    const double Ex =  double(Task::GetInstance().E.xCoord);
    const double Ey =  double(Task::GetInstance().E.yCoord);
    double visitedNum = 0.0;
    double dist = 0.0;
    for ( int i=0; i<row; i++) {
        for ( int j=0; j<col; j++) {
            if (visited[i][j]){
                dist += std::abs(i-Ex+1.0) + std::abs(j-Ey+1.0);
                visitedNum++;
            }
        }
    }

    //magiScore = 2/(átlagos relativ távolság) + relativ kitöltöttség
    double magicScore = ((row+col)*visitedNum/dist + visitedNum/row/col)/2;
    return magicScore;

}

