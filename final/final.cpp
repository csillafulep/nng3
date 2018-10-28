#include<iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <cstddef>
#include <cmath>
#include <algorithm>


enum class Direction
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

static constexpr Direction AllDirections[] = 
    {Direction::UP, Direction::DOWN, Direction::LEFT, Direction::RIGHT};

static constexpr char DirChar[] =
    {'^', 'v', '<', '>'};


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

//Task
class Task
{
public:
    static Task& GetInstance();
    void Initialize(const Input& input);
    int tableRowCount;
    int tableColCount;
    Point E;

private:
    Task() = default;
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
    Point nextPosition(const Direction , const Point& ) const;
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


//Input
Input::Input(std::istream& stream)
{
    ReadHeader(stream);
    ReadValues(stream);
}

Input Input::FromFile(const std::string& filename)
{
    std::ifstream infile(filename);
    return Input(infile);
}

void Input::ReadHeader(std::istream& stream)
{
    std::string line;
    std::getline(stream, line);
    const auto spacePosition = line.find(' ');
    numberOfRows = std::stoi(line.substr(0, spacePosition));
    numberOfCols = std::stoi(line.substr(spacePosition + 1, std::string::npos));
}

void Input::ReadValues(std::istream& stream)
{
    std::string line;
    while(std::getline(stream, line) || values.size() < size_t(numberOfRows)) {
        values.push_back(line);
    }
}


std::ostream& operator<<(std::ostream& os, const Input& input)
{
    os << "rows: " << input.numberOfRows << ", cols: " << input.numberOfCols << "\n";
    for (const auto& line : input.values) {
        os << line << std::endl;
    }

    return os;
}

//Point
Point::Point(int x, int y){
    xCoord = x;
    yCoord = y;
}

//comparison of points
bool Point::operator==(const Point& other) const
{
    return xCoord == other.xCoord && yCoord == other.yCoord;
}

//comparison of points
bool Point::operator!=(const Point& other) const
{
    return !operator==(other);
}

//global: Task
Task& Task::GetInstance()
{
    static Task instance;
    return instance;
}

void Task::Initialize(const Input& input)
{
    tableRowCount = input.numberOfRows;
    tableColCount = input.numberOfCols;

    for (int i = 0; i< input.numberOfRows; i ++){
        for (int j =0; j< input.numberOfCols; j++){
            if (input.values[i][j] == 'E'){
                E.xCoord = i+1;
                E.yCoord = j+1;
            }
        }
    }
}

//Agent
Agent::Agent (Point& position, int fuel)
    : position(position)
    , fuel(fuel)
{
}

//Move
Move::Move (const Agent& agent, Direction dir)
    : movingAgent(agent)
    , direction(dir)
{
}

void Move::plotMove() const{
    std::cout << movingAgent.position.xCoord << " " <<movingAgent.position.yCoord <<  " " << DirChar[(int)direction] <<std::endl;

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


//Board
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
            if (!isOnEdge(dir, agent.position) && nextPosition(dir,agent.position) != agent.position){
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

//calculate magic Score
double Board::calculateMagicScore()  const{
    const double row = double(Task::GetInstance().tableRowCount);
    const double col = double(Task::GetInstance().tableColCount);
    const double Ex =  double(Task::GetInstance().E.xCoord);
    const double Ey =  double(Task::GetInstance().E.yCoord);
    double visitedNum = 0.0;
    double distE = 0.0;
    
    for ( int i=0; i<row; i++) {
        for ( int j=0; j<col; j++) {
            
            if (visited[i][j]){
                distE += std::abs(i-Ex+1.0) + std::abs(j-Ey+1.0);
                visitedNum++;
                
            }
        }
    }

    //magiScore = 2/(átlagos relativ távolság) + relativ kitöltöttség: 
    //double magicScore = ((row+col)*visitedNum/dist + visitedNum/row/col)/2;
    double magicScore = (row+col)*visitedNum/distE;
    //std::cout << "E: " << (row+col)*visitedNum/distE << std::endl;
    return magicScore;

}

//select highest score: magicscore + exploration
 Node* Node::select() {
    size_t iMax = 0U;
    double maxScore = children[0].magicScore/children[0].numberOfGames + 0.01*std::sqrt(2*std::log(numberOfGames)/children[0].numberOfGames);
    for (size_t i = 1U; i < children.size(); ++i) {
        if (maxScore < children[i].magicScore/children[i].numberOfGames + 0.01*std::sqrt(2*std::log(numberOfGames)/children[i].numberOfGames)) {
            iMax = i;
            maxScore = children[i].magicScore/children[i].numberOfGames + 0.01*std::sqrt(2*std::log(numberOfGames)/children[i].numberOfGames);
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


int main()
{
     //read input
    const Input input(std::cin);
    // const auto input = Input::FromFile("../input/7.txt");
    //std::cout << "input: " << input << std::endl;

    //init
    Task::GetInstance().Initialize(input);
    Board initialBoard(input);
    

    Node rootNode;
    Node* currentNode;
    Board currentBoard = initialBoard;
    bool taskSolved = false;
    int numTrial = 1;
    std::vector<Move> solution;

    while ( !taskSolved && numTrial <100000){
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

        std::cout << solution.size() << std::endl;
        for (const auto& move: solution){
            move.plotMove();
        }
    }

    return 0;

}