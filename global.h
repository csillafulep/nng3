#pragma once

#include "header.h"

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

