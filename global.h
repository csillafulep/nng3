#pragma once

#include "header.h"

class Task
{
public:
    static Task& GetInstance();
    int tableRowCount;
    int tableColCount;
    Point E;

private:
    Task() = default;
};

