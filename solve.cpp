#include "header.h"
#include "global.h"
#include <cmath>
#include<iostream>


//calculate magic Score
double Board::calculateMagicScore()  const{
    const double row = double(Task::GetInstance().tableRowCount);
    const double col = double(Task::GetInstance().tableColCount);
    const double Ex =  double(Task::GetInstance().E.xCoord);
    const double Ey =  double(Task::GetInstance().E.yCoord);
    double visitedNum = 0.0;
    double distE = 0.0;
    double distAgents = 0.0;
    for ( int i=0; i<row; i++) {
        for ( int j=0; j<col; j++) {
            
            if (visited[i][j]){
                distE += std::abs(i-Ex+1.0) + std::abs(j-Ey+1.0);
                visitedNum++;
                if (!occupied[i][j]){
                    double minAgentDist = row + col;
                    for ( const auto& agent : agentsOnBoard){
                         if ((std::abs(i-agent.position.xCoord+1) + std::abs(j-agent.position.yCoord+1)) < minAgentDist)
                            minAgentDist = std::abs(i-agent.position.xCoord+1) + std::abs(j-agent.position.yCoord+1);
                    }
                    distAgents += minAgentDist;
                }
            }
        }
    }

    //magiScore = 2/(átlagos relativ távolság) + relativ kitöltöttség: 
    //double magicScore = ((row+col)*visitedNum/dist + visitedNum/row/col)/2;
    double magicScore = (row+col)*visitedNum/distE + agentsOnBoard.size()*visitedNum/distAgents;
    //std::cout << "E: " << (row+col)*visitedNum/distE << ", agent: " << agentsOnBoard.size()*visitedNum/distAgents << std::endl;
    return magicScore;

}

