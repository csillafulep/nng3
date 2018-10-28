#include "header.h"
#include "global.h"
#include <cmath>
#include<iostream>


//calculate magic Score
double Board::calculateMagicScore(const Board& preivousBoard)  const{
    const double row = double(Task::GetInstance().tableRowCount);
    const double col = double(Task::GetInstance().tableColCount);
    const double Ex =  double(Task::GetInstance().E.xCoord);
    const double Ey =  double(Task::GetInstance().E.yCoord);
    int visitedNum = 0.0;
    double distE = 0.0;
    double dist = 0.0;
    for ( int i=0; i<row; i++) {
        for ( int j=0; j<col; j++) {
            
            if (visited[i][j] && !preivousBoard.visited[i][j]){
                distE = std::abs(i-Ex+1.0) + std::abs(j-Ey+1.0);
                visitedNum++;
                for ( const auto& agent : agentsOnBoard){
                    
                    if ((std::abs(i-agent.position.xCoord+1) + std::abs(j-agent.position.yCoord+1)) < distE )
                        distE = std::abs(i-agent.position.xCoord+1) + std::abs(j-agent.position.yCoord+1);
                }

                dist += distE;
            }
        
        }
    }

    
    //magiScore = 2/(átlagos relativ távolság) + relativ kitöltöttség: 
    //double magicScore = ((row+col)*visitedNum/dist + visitedNum/row/col)/2;
    double magicScore = (row+col-dist/visitedNum)/(row+col);
    //std::cout << (row+col)*visitedNum/dist << std::endl;
    return magicScore;

}

