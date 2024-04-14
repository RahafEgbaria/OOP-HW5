#ifndef PART2_RUSHHOUR_H
#define PART2_RUSHHOUR_H

#include "List.h"
#include "Utilities.h"
#include "CellType.h"
#include "Direction.h"
#include "BoardCell.h"
#include "GameBoard.h"
#include "MoveVehicle.h"

/********************** GetLocation begin **********************/
template <typename, int, int, CellType>
struct GetLocation;

template <typename B, int R, int C, CellType type>
struct GetLocation
{
    // find the cell
    typedef typename GetAtIndex<C, typename GetAtIndex<R, typename B::board>::value>::value cell;

    // check if this is the wanted car. If no, search in the prev col
    typedef typename Conditional<(cell::type == type),
                                 Location<R, C>,
                                 typename GetLocation<B, R, C - 1, type>::location>::value location;
};

// search in the next row
template <typename B, int R, CellType type>
struct GetLocation<B, R, -1, type>
{
    typedef typename GetLocation<B, R - 1, B::width - 1, type>::location location;
};

template <typename B, int C, CellType type>
struct GetLocation<B, -1, C, type>
{
    static_assert(false, "WE ARE NOT SUPPOSDED TO GET HERE!!");
};

// note: the car should be found
/********************** GetLocation end **********************/
/********************** CheckWin begin **********************/
template <typename, int, int>
struct CheckIfCanExit;

template <typename B, int R, int C>
struct CheckIfCanExit
{
    // find the cell
    typedef typename GetAtIndex<C, typename GetAtIndex<R, typename B::board>::value>::value cell;

    /*static constexpr bool value = (type::value == X || type::value == EMPTY) && (B::width - (C+1) > 0) ?
                                  CheckIfCanExit<B, C+1, B::width>::value : (type::value == X || type::value == EMPTY);*/

    typedef typename Conditional<(cell::type == X || cell::type == EMPTY) && (B::width - (C + 1) > 0),
                                 typename CheckIfCanExit<B, C + 1, B::width>::ans,
                                 typename Conditional<(B::width - (C + 1) > 0), std::false_type, std::true_type>::value>::value value;
};
/**************************/
template <typename>
struct CheckWin;

template <typename B>
struct CheckWin
{
    // find the location of the red car
    typedef typename GetLocation<B, B::length, B::width, X>::location location;

    // check if it can exit the grid
    typedef typename CheckIfCanExit<B, location::row, location::col>::value result;
};
/********************** CheckWin end **********************/
/********************** CheckSolution begin **********************/
template <typename, typename>
struct CheckSolution;

/*template<typename B, typename L>
struct Execute{
    //get the curr step to execute
    typedef typename L::head curr;

    //get the location of the car
    typedef typename GetLocation<B, B::length-1, B::width-1, curr::type>::value location;

    //move the car
    typedef typename MoveVehicle<B, location::row, location::col, curr::direction, curr::amount>::board updated_board;

    //check solution
    typedef typename CheckSolution<updated_board, typename L::next>::result tmp;
};*/

/*template<typename B, typename L>
struct CheckSolution {
    //static constexpr bool result = (L::size == 0) ? CheckWin<B>::value : Execute<B, L>::tmp;
    //typedef typename Conditional<(L::size == 0), CheckWin<B>, Execute<B, L>>::value::result result;
    static constexpr bool result = Conditional<(L::size == 0), typename CheckWin<B>::result, typename Execute<B, L>::tmp>::value;
};*/

template <typename B, typename L>
struct CheckSolution
{
    // get the curr step to execute
    typedef typename L::head curr;

    // get the location of the car
    typedef typename GetLocation<B, B::length - 1, B::width - 1, curr::type>::location location;

    // move the car
    typedef typename MoveVehicle<B, location::row, location::col, curr::direction, curr::amount>::board updated_board;

    // check solution
    static constexpr bool result = CheckSolution<updated_board, typename L::next>::result;
};

template <typename B>
struct CheckSolution<B, List<>>
{
    static constexpr bool result = CheckWin<B>::result::value;
};
/********************** CheckSolution end **********************/

#endif // PART2_RUSHHOUR_H