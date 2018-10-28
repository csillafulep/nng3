#include "header.h"
#include <algorithm>
#include <cmath>
#include <iostream>

//select highest score: magicscore + exploration
 Node* Node::select() {
    size_t iMax = 0U;
    double maxScore = children[0].magicScore/children[0].numberOfGames + std::sqrt(2*std::log(numberOfGames)/children[0].numberOfGames) ;
    for (size_t i = 1U; i < children.size(); ++i) {
        if (maxScore < children[i].magicScore/children[i].numberOfGames + std::sqrt(2*std::log(numberOfGames)/children[i].numberOfGames) ) {
            iMax = i;
            maxScore = children[i].magicScore/children[i].numberOfGames + std::sqrt(2*std::log(numberOfGames)/children[i].numberOfGames) ;
        }
    }

    return &children[iMax];
    
 }


//expand leaf
bool Node::expand( const Board& currentBoard){
    bool success = false;
    auto moves = currentBoard.possibleMoves();
    for (const auto& move: moves){
        auto newBoard = currentBoard;
        newBoard.applyMove(move);
        if (newBoard.success())
            success = true;

        Node newNode;
        newNode.magicScore = newBoard.calculateMagicScore(currentBoard);
        newNode.numberOfGames = 1;
        newNode.previousMove = move;
        newNode.parent = this;
        children.push_back(newNode);
    }
    return success;
}

//escaleta scores update
void Node::EscalateUpdate()
{
    int games = 1;
    int newMagicScore = 0;
    if (children.empty()){
        games = 100;
    }

    else {
        games = children.size();
        for (const auto& child : children) {
            newMagicScore += child.magicScore;
        }
    }

    Node* node = this;
    while(node) {
        node->magicScore += newMagicScore; 
        node->numberOfGames += games;
        node = node->parent;
    }
}