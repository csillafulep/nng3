#pragma once

enum class Direction
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

static constexpr Direction AllDirections[] = 
    {Direction::UP, Direction::DOWN, Direction::LEFT, Direction::RIGHT};