#include<iostream>
#include "header.h"
#include "global.h"

int main()
{
    //read input
    const auto input = Input::FromFile("../input/5.txt");
    std::cout << "input: " << input << std::endl;

    //init
    Task::GetInstance().Initialize(input);
    std::cout << "E(" << Task::GetInstance().E.xCoord << ";" << Task::GetInstance().E.yCoord << ")" << std::endl;
    
    Board initialBoard(input);
    initialBoard.plotBoard();


    Node rootNode;
    Node* currentNode;
    Board currentBoard = initialBoard;
    bool taskSolved = false;
    int numTrial = 1;
    std::vector<Move> solution;

    while ( !taskSolved && numTrial <10000){
        std::cout << "--------------------" << numTrial << "-------------" << std::endl;
        currentBoard = initialBoard;
        currentNode = &rootNode;

        while(!currentNode->children.empty()){
            currentNode = currentNode->select();
            currentBoard.applyMove(currentNode->previousMove);
            //currentNode->previousMove.plotMove();
            //currentBoard.plotBoard();
        }

        taskSolved = currentNode->expand(currentBoard);
        currentNode->EscalateUpdate();        
        numTrial++;
    }


    std::cout << "--------------------" << std::endl;
    std::cout << "success: " << taskSolved << std::endl;
    std::cout << "number of trials: " << numTrial << std::endl;
    std::cout << "SOLUTION: " << std::endl;

    //solution
    if (taskSolved){
        auto moves = currentBoard.possibleMoves();
        taskSolved = false;
        int ind = 0;
        while (!taskSolved){
            auto newBoard = currentBoard;
            newBoard.applyMove(moves[ind]);
            if (newBoard.success()){
                solution.push_back(moves[ind]);
                taskSolved = true;
            }
            ind++;
        }
        
        while(currentNode->parent!=nullptr){
            solution.insert(solution.begin(),currentNode->previousMove);
            currentNode = currentNode->parent;
        }

        for (const auto& move: solution){
            move.plotMove();
        }
    }

    return 0;
}
