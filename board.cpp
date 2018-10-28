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
            if (!isOnEdge(dir, agent.position) ){
                Move nextOption( agent,  dir);
                moveOptions.push_back (nextOption);
            }
            
        }

    }

  return moveOptions;  
}

//possibility to move in a direction on athe board
Point Board::nextPosition(const Direction dir, const Point& p) const {
    Point pNew = p;
    if (dir == Direction::DOWN){
        pNew.xCoord = p.xCoord + 1;
    }
    else if (dir == Direction::UP){
        pNew.xCoord = p.xCoord - 1;
    }
    
    else if (dir == Direction::RIGHT){
        pNew.yCoord = p.yCoord + 1;
    }
    else if (dir == Direction::LEFT){
        pNew.yCoord = p.yCoord - 1;
    }
    return pNew;
}

//apply agent's move on board
void Board::applyMove (const Move& nextMove){
    Point oldPos = nextMove.movingAgent.position;
    Point newPos = nextPosition(nextMove.direction, oldPos);
    int agentInd = find_agent(nextMove.movingAgent);
    int currentFuel = agentsOnBoard[agentInd].fuel;

    occupied[oldPos.xCoord-1][oldPos.yCoord-1] = false;
                
    while(currentFuel>0 && !isOnEdge(nextMove.direction, oldPos)){
        if (visited[newPos.xCoord-1][newPos.yCoord-1] == false){
            currentFuel--;
            visited[newPos.xCoord-1][newPos.yCoord-1] = true;
        }
                
        //next step
        oldPos = newPos;
        newPos = nextPosition(nextMove.direction, oldPos);

    }

    agentsOnBoard.erase(agentsOnBoard.begin()+agentInd);
        

}

//E already reached
bool Board::success(){
    return visited[Task::GetInstance().E.xCoord-1][Task::GetInstance().E.yCoord-1];
}