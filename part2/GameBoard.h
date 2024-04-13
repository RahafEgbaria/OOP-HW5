#ifndef PART2_GAMEBOARD_H
#define PART2_GAMEBOARD_H

#include "List.h"

/********************** GameBoard begin **********************/
template<typename>
struct GameBoard;

template<typename... UU, typename... TT>
struct GameBoard<List<List<UU...>, TT...>> {
    typedef List<List<UU...>, TT...> board;
    static constexpr int width = board::head::size; //number of cols
    static constexpr int length = board::size; //number of rows
};
/********************** GameBoard begin **********************/

#endif //PART2_GAMEBOARD_H