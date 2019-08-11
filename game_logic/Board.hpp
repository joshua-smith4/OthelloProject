#ifndef BOARD_HPP_INCLUDED
#define BOARD_HPP_INCLUDED

#include <vector>
#include <array>

enum class CoordState
{
    EMPTY,
    WHITE,
    BLACK
};

std::ostream& operator<<(std::ostream&, CoordState const&);

class Board
{
public:
    Board(unsigned, unsigned);
    Board(Board const&);
    CoordState getCoordState(unsigned x, unsigned y) const;
    CoordState getCoordState(unsigned i) const;

    std::vector<unsigned> getAdjacentIndices(unsigned x, unsigned y) const;
    std::vector<unsigned> getLineIndices(unsigned x, unsigned y, int modx, int mody) const;

    const unsigned X_DIM;
    const unsigned Y_DIM;
    unsigned coordsToIndex(unsigned x, unsigned y) const;

    std::vector<CoordState> spaces;
};

std::ostream& operator<<(std::ostream&, Board const&);

struct Move
{
    unsigned x;
    unsigned y;
    CoordState player;
};

std::ostream& operator<<(std::ostream&, Move const&);

#endif
