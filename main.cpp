#include<iostream>
#include "header.h"
#include "global.h"

int main()
{
    const auto input = Input::FromFile("../input/1.txt");
    std::cout << "input: " << input << std::endl;

    Task::GetInstance().Initialize(input);
    std::cout << "E(" << Task::GetInstance().E.xCoord << ";" << Task::GetInstance().E.yCoord << ")" << std::endl;
    
    Board initialBoard(input);
    initialBoard.plotBoard();
    Node rootNode;

    Board currentBoard = initialBoard;
    rootNode.expand(currentBoard);
    rootNode.EscalateUpdate();

    Node node1 = rootNode.select();
    currentBoard.applyMove(node1.previousMove);
    node1.expand(currentBoard);

    std::cout << "success: " << initialBoard.success()<< std::endl;

    return 0;
}
