#pragma once
#include "direction.h"
#include <vector>
#include <iostream>

class Input
{
public:
    static Input FromFile(const std::string& filename);
    Input(std::istream&);

    int numberOfRows;
    int numberOfCols;
    std::vector<std::string> values;

private:
    void ReadHeader(std::istream&);
    void ReadValues(std::istream&);
};

class Point{
    public:
    int xCoord;
    int yCoord;
    Point() = default;
    Point(int, int);

    bool operator==(const Point& other) const;
    bool operator!=(const Point& other) const;
};

class Agent{
    public:
    Point position;
    int fuel;
    Agent() = default;
    Agent(Point& , int );
};

class Move{
    public:
    Agent movingAgent;
    Direction direction; 
    Move() = default;
    Move(const Agent& , Direction);
    void plotMove() const;
};

class Board{
    public:
    std::vector<std::vector<bool>> visited;
    std::vector<std::vector<bool>> occupied;
    std::vector<Agent> agentsOnBoard; 

    Board(const Input& input);
    // possible moves on board
    std::vector<Move>  possibleMoves() const;
    //calculate magicScore of a board
    double calculateMagicScore() const;
    // possibility to movi in a direction on the board
    Point nextPosition(const Direction , const Point& );
    //find out which agent moves
    int find_agent (const Agent&) const;
    //apply agent's move on Board
    void applyMove (const Move&);
    // plot current board to std output
    void plotBoard() const;
    //E already reached
    bool success();
};

class Node{
public:
    double magicScore;
    int numberOfGames = 0;
    Move previousMove;
    Node* parent = nullptr;
    std::vector<Node> children;

    //escalate scores update
    void EscalateUpdate();
    //expand leaf
    bool expand( const Board& );
    //select highest score: magicscore + exploration
    Node* select();
};


//agent is on the edge of the board
bool isOnEdge(const Direction , const Point&);


std::ostream& operator<<(std::ostream&, const Input&);
