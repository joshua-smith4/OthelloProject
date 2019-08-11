#include "Game.hpp"
#include <cassert>
#include <iostream>

bool Rules::isValidLine(Board const& b, std::vector<unsigned> const& l, CoordState const& player)
{
    for(auto i = 0u; i < l.size(); ++i)
    {
        auto cur_coord = b.getCoordState(l[i]);
        if(i == 0u && cur_coord != CoordState::EMPTY) break;
        if(i == 1u && cur_coord == player) break;
        if(cur_coord == CoordState::EMPTY && i != 0u) break;
        if(cur_coord == player) return true;
    }
    return false;
}

bool Rules::moveNotEmpty(Board const& b, Move const& m, CoordState const& c)
{
    return m.player != CoordState::EMPTY && m.player == c;
}

bool Rules::isInBounds(Board const& b, Move const& m, CoordState const&)
{
    return m.x < b.X_DIM && m.y < b.Y_DIM;
}

bool Rules::nextToOtherColor(Board const& b, Move const& m, CoordState const&)
{
    const auto other = m.player == CoordState::WHITE ? CoordState::BLACK : CoordState::WHITE;
    for(auto&& space : b.getAdjacentIndices(m.x,m.y))
    {
        if(b.getCoordState(space) == other) return true;
    }
    return false;
}

bool Rules::isEmptySpace(Board const& b, Move const& m, CoordState const&)
{
    return b.getCoordState(m.x,m.y) == CoordState::EMPTY;
}

bool Rules::canFlank(Board const& b, Move const& m, CoordState const&)
{
    for(auto&& dir : Rules::directions)
    {
        auto line = b.getLineIndices(m.x, m.y, dir.first, dir.second);
        if(Rules::isValidLine(b, line, m.player)) return true;
    }
    return false;
}

const std::vector<Rules::rules_function> Rules::rules = {
    Rules::moveNotEmpty,
    Rules::isInBounds,
    Rules::nextToOtherColor,
    Rules::isEmptySpace,
    Rules::canFlank
};

const std::map<Rules::rules_function, std::string> Rules::error_msg = {
    std::make_pair(moveNotEmpty, "It is either not your turn or the move is invalid due to being empty."),
    std::make_pair(isInBounds, "The move is invalid due to being outside the board bounds."),
    std::make_pair(nextToOtherColor, "All moves must be adjacent to an opponent's tile."),
    std::make_pair(isEmptySpace, "The indicated space is not empty."),
    std::make_pair(canFlank, "All moves must flank the opponent.")
};

const std::array<std::pair<int,int>, 8> Rules::directions = {
    std::make_pair(-1,0), // left
    std::make_pair(-1,-1), // left up
    std::make_pair(0,-1), // up
    std::make_pair(1,-1), // right up
    std::make_pair(1,0), // right
    std::make_pair(1,1), // right down
    std::make_pair(0,1), // down
    std::make_pair(-1,1) // left down
};

Game::Game(unsigned x_dim, unsigned y_dim) : board(x_dim, y_dim), turn(CoordState::BLACK)
{
    assert(x_dim > 2 && x_dim % 2 == 0 && y_dim > 2 && y_dim % 2 == 0);
    unsigned xmid = board.X_DIM/2-1;
    unsigned ymid = board.Y_DIM/2-1;
    board.spaces[board.coordsToIndex(xmid,ymid)] = CoordState::WHITE;
    board.spaces[board.coordsToIndex(xmid+1,ymid+1)] = CoordState::WHITE;
    board.spaces[board.coordsToIndex(xmid,ymid+1)] = CoordState::BLACK;
    board.spaces[board.coordsToIndex(xmid+1,ymid)] = CoordState::BLACK;

    assert(Rules::rules.size() == Rules::error_msg.size());
}

Game::Game(Board b, CoordState t) : board(b), turn(t) {}

std::pair<bool, std::string> Game::isMoveValid(Move const& m) const
{
    for(auto&& func : Rules::rules)
    {
        if(!func(board, m, turn))
            return std::make_pair(false, Rules::error_msg.at(func));
    }
    return std::make_pair(true, "");
}

bool Game::canPlayerMove(CoordState const& p) const
{
    for(auto i = 0u; i < board.Y_DIM; ++i)
    {
        for(auto j = 0u; j < board.X_DIM; ++j)
        {
            if(isMoveValid({j,i,p}).first) return true;
        }
    }
    return false;
}

Board Game::getBoard() const
{
    return board;
}

CoordState Game::getCurrentTurn() const
{
    return turn;
}

CoordState Game::getLeader() const
{
    unsigned num_black = 0u, num_white = 0u;
    for(auto&& elem : board.spaces)
    {
        if(elem == CoordState::WHITE) ++num_white;
        else if(elem == CoordState::BLACK) ++num_black;
    }
    if(num_black == num_white) return CoordState::EMPTY;
    return num_black > num_white ? CoordState::BLACK : CoordState::WHITE;
}

std::ostream& operator<<(std::ostream& os, Game const& g)
{
    os << "Player Turn: " << g.getCurrentTurn() << "\n";
    os << g.getBoard();
    return os;
}
