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

    return 0;
}
