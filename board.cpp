#include "header.h"
#include <cstddef>
#include<iostream>
#include "global.h"


Board::Board(const Input& input){
    visited = std::vector<std::vector<bool>>(input.numberOfRows, std::vector<bool>(input.numberOfCols, false));
    occupied = std::vector<std::vector<bool>>(input.numberOfRows, std::vector<bool>(input.numberOfCols, false));
    for (int i = 0; i< input.numberOfRows; i ++){
        for (int j =0; j< input.numberOfCols; j++){
            if (int(input.values[i][j]-'0')>0 && int(input.values[i][j]-'0')<10){
                Point pos(i+1,j+1);
                int fuel = int(input.values[i][j]-'0');
                Agent newAgent(pos, fuel);
                agentsOnBoard.push_back(newAgent);
                visited[i][j] = true;
                occupied[i][j] = true;
            }
        }
    }
}


// plot current board to std output
void Board::plotBoard() const{
    const int row = Task::GetInstance().tableRowCount;
    const int col = Task::GetInstance().tableColCount;
    for (int i = 0; i<row; i++){
        for (int j = 0; j < col; j++){
            if (occupied[i][j]){
                for (int ind = 0; ind < agentsOnBoard.size(); ind ++){
                    if (agentsOnBoard[ind].position.xCoord == i+1  && agentsOnBoard[ind].position.yCoord == j+1)
                        std::cout << agentsOnBoard[ind].fuel << " ";
                }
    
            }
            else if (visited[i][j])
                std::cout << "+ ";
            else
                std::cout << ". ";
        }
        std::cout << std::endl;
    }
}



//find out which agent moves
int Board::find_agent (const Agent& agent) const{
    int agentIndx;
    for (int ind = 0; ind < agentsOnBoard.size(); ind ++){
        if (agentsOnBoard[ind].position == agent.position  && agentsOnBoard[ind].fuel == agent.fuel )
            agentIndx = ind;
    }
    return agentIndx;
}


//possible moves on board
std::vector<Move>  Board::possibleMoves() const{
    std::vector<Move> moveOptions;
    for (const auto& agent : agentsOnBoard) {
        for (const auto dir : AllDirections){
            if (!isOnEdge(dir, agent.position)){
                Move nextOption( agent,  dir);
                moveOptions.push_back (nextOption);
            }
            
        }

    }

  return moveOptions;  
}

//possibility to move in a direction on athe board
Point Board::nextPosition(const Direction dir, const Point& p){
    int xPos = p.xCoord;
    int yPos = p.yCoord;
    Point pNew = p;
    if (dir == Direction::DOWN){
        while( xPos<Task::GetInstance().tableRowCount && occupied[xPos][yPos-1])
            xPos++;
        if (xPos != Task::GetInstance().tableRowCount)
            pNew.xCoord = xPos + 1;
    }
    else if (dir == Direction::UP){
        while( xPos>1 && occupied[xPos-2][yPos-1])
            xPos--;
        if (xPos != 1)
            pNew.xCoord = xPos -1;
    }
    
    else if (dir == Direction::RIGHT){
        while( yPos<Task::GetInstance().tableColCount && occupied[xPos-1][yPos])
            yPos++;
        if (yPos != Task::GetInstance().tableColCount)
            pNew.yCoord = yPos + 1;
    }
    else if (dir == Direction::LEFT){
        while( yPos>1 && occupied[xPos-1][yPos-2])
            yPos--;
        if (yPos != 1)
            pNew.yCoord = yPos-1;
    }
    return pNew;
}

//apply agent's move on board
void Board::applyMove (const Move& nextMove){
    Point oldPos = nextMove.movingAgent.position;
    Point newPos = nextPosition(nextMove.direction, oldPos);
    int agentInd = find_agent(nextMove.movingAgent);
    int currentFuel = agentsOnBoard[agentInd].fuel;
        
    while(currentFuel>0 && newPos != oldPos){
        if (visited[newPos.xCoord-1][newPos.yCoord-1] == false){
            currentFuel--;
            visited[newPos.xCoord-1][newPos.yCoord-1] = true;
        }
        occupied[oldPos.xCoord-1][oldPos.yCoord-1] = false;
        
        //next step
        oldPos = newPos;
        newPos = nextPosition(nextMove.direction, oldPos);

    }

    if(currentFuel == 0)
            agentsOnBoard.erase(agentsOnBoard.begin()+agentInd);
    else {
            agentsOnBoard[agentInd].position = newPos;
            agentsOnBoard[agentInd].fuel = currentFuel;  
            occupied[newPos.xCoord-1][newPos.yCoord-1] = true;
        }

}

//E already reached
bool Board::success(){
    return visited[Task::GetInstance().E.xCoord-1][Task::GetInstance().E.yCoord-1];
}