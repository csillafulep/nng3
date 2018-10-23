#include "global.h"

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