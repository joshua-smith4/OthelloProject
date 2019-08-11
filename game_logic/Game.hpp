#ifndef GAME_HPP_INCLUDED
#define GAME_HPP_INCLUDED

#include "Board.hpp"
#include <vector>
#include <map>
#include <string>
#include <array>

namespace Rules
{
    using rules_function = bool(*const)(Board const&, Move const&, CoordState const&);

    bool isValidLine(Board const&, std::vector<unsigned> const&, CoordState const&);

    bool moveNotEmpty(Board const&, Move const&, CoordState const&);
    bool isInBounds(Board const&, Move const&, CoordState const&);
    bool nextToOtherColor(Board const&, Move const&, CoordState const&);
    bool isEmptySpace(Board const&, Move const&, CoordState const&);
    bool canFlank(Board const&, Move const&, CoordState const&);

    extern const std::vector<rules_function> rules;
    extern const std::map<rules_function, std::string> error_msg;
    extern const std::array<std::pair<int,int>, 8> directions;
}

class Game
{
public:
    Game(unsigned, unsigned);
    Game(Board, CoordState);
    std::pair<bool, std::string> isMoveValid(Move const&) const;
    bool canPlayerMove(CoordState const&) const;
    std::pair<bool, std::string> applyMove(Move const&);
    Board getBoard() const;
    CoordState getCurrentTurn() const;
    CoordState getLeader() const;
private:
    Board board;
    CoordState turn;
};

std::ostream& operator<<(std::ostream&, Game const&);

#endif
