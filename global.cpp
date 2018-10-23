#include "global.h"

Task& Task::GetInstance()
{
    static Task instance;
    return instance;
}
