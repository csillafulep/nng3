#include "header.h"
#include <algorithm>
#include <cmath>

//select highest score: magicscore + exploration
 Node& Node::select() {
    size_t iMax = 0U;
    double maxScore = children[0].magicScore + std::sqrt(std::log(numberOfGames)/children[0].numberOfGames);
    for (size_t i = 1U; i < children.size(); ++i) {
        if (maxScore < children[i].magicScore + std::sqrt(std::log(numberOfGames)/children[i].numberOfGames)) {
            iMax = i;
            maxScore = children[i].magicScore + std::sqrt(std::log(numberOfGames)/children[i].numberOfGames);
        }
    }

    return children[iMax];
    
 }


//expand leaf
void Node::expand( const Board& currentBoard){
    auto moves = currentBoard.possibleMoves();
    for (const auto& move: moves){
        move.plotMove();
        auto newBoard = currentBoard;
        newBoard.applyMove(move);
        newBoard.plotBoard();
        Node newNode;
        newNode.magicScore = newBoard.calculateMagicScore();
        newNode.numberOfGames = 1;
        newNode.previousMove = move;
        newNode.parent = this;
        children.push_back(newNode);
    }
    
}

//escaleta scores update
void Node::EscalateUpdate()
{
    int magicScoreSum = 0;
    for (const auto& child : children) {
        magicScoreSum += child.magicScore;
    }

    int games = std::max(int(children.size()),1);
    
    Node* node = this;
    while(node) {
        node->magicScore += magicScoreSum; 
        node->numberOfGames += games;
        node = node->parent;
    }
}