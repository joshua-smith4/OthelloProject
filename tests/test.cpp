#include <cassert>
#include <iostream>
#include "Game.hpp"

int main()
{
    Game game(8,8);
    //std::cout << game << "\n";
    assert(game.getCurrentTurn() == CoordState::BLACK);
    for(auto i = 0u; i < game.getBoard().Y_DIM; ++i)
    {
        for(auto j = 0u; j < game.getBoard().X_DIM; ++j)
        {
            auto moveBlack = Move({j,i,CoordState::BLACK});
            auto moveWhite = Move({j,i,CoordState::WHITE});
            auto moveEmpty = Move({j,i,CoordState::EMPTY});
            auto resBlack = game.isMoveValid(moveBlack);
            auto resWhite = game.isMoveValid(moveWhite);
            auto resEmpty = game.isMoveValid(moveEmpty);

            //std::cout << moveBlack << " | " << std::boolalpha << resBlack.first << ": " << resBlack.second << "\n";
            //std::cout << moveWhite << " | " << std::boolalpha << resWhite.first << ": " << resWhite.second << "\n";
            //std::cout << moveEmpty << " | " << std::boolalpha << resEmpty.first << ": " << resEmpty.second << "\n";

            assert(resWhite.first == false);
            assert(resEmpty.first == false);
            if(j == 3 && i == 2) assert(resBlack.first == true);
            else if(j == 2 && i == 3) assert(resBlack.first == true);
            else if(j == 4 && i == 5) assert(resBlack.first == true);
            else if(j == 5 && i == 4) assert(resBlack.first == true);
            else assert(resBlack.first == false);
        }
    }
    Board b(8,8);
    for(auto&& elem : b.spaces) elem = CoordState::BLACK;
    Game g2(b, CoordState::WHITE);
    assert(g2.canPlayerMove(g2.getCurrentTurn()) == false);
    return 0;
}
