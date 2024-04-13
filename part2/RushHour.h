#ifndef PART2_RUSHHOUR_H
#define PART2_RUSHHOUR_H

#include "RushHourAux.h"

/********************** CheckWin begin **********************/
template <typename B>
struct CheckWin;

template <typename B>
struct CheckWin
{
    // find the location of the red car
    typedef typename FindCar<B, B::length - 1, B::width - 1, X>::value location;

    // check if it can exit the grid
    static constexpr bool result = CheckIfCanExit<B, location::row, location::col>::value::result;
    // static constexpr bool result = CheckIfCanExit<B, location::row, location::col, (B::width - 1) - C>::value;
};
/********************** CheckWin end **********************/
/********************** CheckSolution begin **********************/
template <typename B, typename L>
struct CheckSolution;

template <typename B, typename L>
struct CheckSolution
{
    // get the step we need to execute
    typedef typename L::head curr;

    // find the car
    typedef typename FindCar<B, B::length - 1, B::width - 1, curr::type>::value location;

    // move the vehicle
    typedef typename MoveVehicle<B, location::row, location::col, curr::direction, curr::amount>::board updated_board;

    // check the solution on the board after the move
    static constexpr bool result = CheckSolution<updated_board, typename L::next>::result;
};

template <typename B>
struct CheckSolution<B, List<>>
{
    static constexpr bool result = CheckWin<B>::result;
};
/********************** CheckSolution end **********************/
#endif // PART2_RUSHHOUR_H