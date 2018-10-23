#include "header.h"
#include <algorithm>
#include <cmath>

//select highest score: magicscore + exploration
 Node* Node::select() {
    size_t iMax = 0U;
    double maxScore = children[0].magicScore + 100*std::sqrt(std::log(numberOfGames)/children[0].numberOfGames);
    for (size_t i = 1U; i < children.size(); ++i) {
        if (maxScore < children[i].magicScore + 100*std::sqrt(std::log(numberOfGames)/children[i].numberOfGames)) {
            iMax = i;
            maxScore = children[i].magicScore + 100*std::sqrt(std::log(numberOfGames)/children[i].numberOfGames);
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
        newNode.magicScore = newBoard.calculateMagicScore();
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
    int magicScoreSum = 0;
    for (const auto& child : children) {
        magicScoreSum += child.magicScore;
    }
    if (children.empty())
        games = 10;
    else
        games = std::max(int(children.size()),1);
    
    Node* node = this;
    while(node) {
        node->magicScore += magicScoreSum; 
        node->numberOfGames += games;
        node = node->parent;
    }
}