#include "header.h"

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