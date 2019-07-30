#include "Board.hpp"
#include <iostream>
#include <algorithm>
#include <cassert>

std::ostream& operator<<(std::ostream& os, CoordState const& cs)
{
    switch(cs)
    {
        case CoordState::EMPTY:
            os << "_";
            break;
        case CoordState::WHITE:
            os << "W";
            break;
        case CoordState::BLACK:
            os << "B";
            break;
    }
    os << " ";
    return os;
}

Board::Board(unsigned x_dim, unsigned y_dim) : X_DIM(x_dim), Y_DIM(y_dim), spaces(x_dim*y_dim, CoordState::EMPTY)
{
}

Board::Board(Board const& b) : X_DIM(b.X_DIM), Y_DIM(b.Y_DIM), spaces(b.spaces) {}

CoordState Board::getCoordState(unsigned x, unsigned y) const { return spaces[coordsToIndex(x,y)]; }

CoordState Board::getCoordState(unsigned i) const { assert(i < X_DIM * Y_DIM); return spaces[i]; }

std::vector<unsigned> Board::getAdjacentIndices(unsigned x, unsigned y) const
{
    std::vector<unsigned> retVal;
    retVal.reserve(8);
    auto checkLeft = x > 0;
    auto checkRight = x < (X_DIM - 1);
    auto checkUp = y > 0;
    auto checkDown = y < (Y_DIM - 1);
    if(checkLeft) retVal.push_back(coordsToIndex(x-1,y));
    if(checkLeft && checkUp) retVal.push_back(coordsToIndex(x-1,y-1));
    if(checkUp) retVal.push_back(coordsToIndex(x,y-1));
    if(checkRight && checkUp) retVal.push_back(coordsToIndex(x+1,y-1));
    if(checkRight) retVal.push_back(coordsToIndex(x+1,y));
    if(checkRight && checkDown) retVal.push_back(coordsToIndex(x+1,y+1));
    if(checkDown) retVal.push_back(coordsToIndex(x,y+1));
    if(checkLeft && checkDown) retVal.push_back(coordsToIndex(x-1,y+1));
    return retVal;
}

std::vector<unsigned> Board::getLineIndices(unsigned x, unsigned y, int modx, int mody) const
{
    std::vector<unsigned> retVal;
    while(x < X_DIM && y < Y_DIM)
    {
        retVal.push_back(coordsToIndex(x,y));
        x += modx;
        y += mody;
    }
    return retVal;
}

unsigned Board::coordsToIndex(unsigned x, unsigned y) const
{
    assert(x < X_DIM && y < Y_DIM);
    return y*X_DIM+x;
}

std::ostream& operator<<(std::ostream& os, Board const& b)
{
    for(auto i = 0u; i < b.Y_DIM; ++i)
    {
        for(auto j = 0u; j < b.X_DIM; ++j)
        {
            os << b.getCoordState(j,i);
        }
        os << "\n";
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, Move const& m)
{
    os << "(" << m.x << ", " << m.y << "): " << m.player;
    return os;
}

