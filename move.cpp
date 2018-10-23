#include "header.h"
#include "global.h"

Move::Move (const Agent& agent, Direction dir)
    : movingAgent(agent)
    , direction(dir)
{
}

void Move::plotMove() const{
    std::cout << "Move: " << movingAgent.position.xCoord << " " <<movingAgent.position.yCoord <<  " " << DirChar[(int)direction] <<std::endl;

}

//agent is on the edge of the board
bool isOnEdge(const Direction dir, const Point& p){
    if (dir == Direction::UP &&  p.xCoord==1)
        return true;

    else if (dir == Direction::DOWN &&  p.xCoord==Task::GetInstance().tableRowCount)
        return true;
    
    else if (dir == Direction::LEFT &&  p.yCoord==1)
        return true;
    
    else if (dir == Direction::RIGHT &&  p.yCoord==Task::GetInstance().tableColCount)
        return true;
    
    else 
        return false;
}
