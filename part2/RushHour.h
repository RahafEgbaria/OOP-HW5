#ifndef PART2_RUSHHOUR_H
#define PART2_RUSHHOUR_H

#include "MoveVehicle.h"
#include "List.h"
#include "Direction.h"
#include "Utilities.h"
#include "GameBoard.h"
#include "CellType.h"
#include "BoardCell.h"

/********************** FindCar begin **********************/
// note: the car SHOULD be found
template <typename B, int R, int C, CellType T>
struct FindCar;

template <typename B, int R, int C, CellType T>
struct FindCar
{
    // find the cell
    typedef typename GetAtIndex<C, typename GetAtIndex<R, typename B::board>::value>::value curr_cell;

    // check if this is the wanted car. If no, search in the prev col
    typedef typename ConditionalAndValue<(curr_cell::type == T),
                                         Location<R, C>,
                                         FindCar<B, R, C - 1, T>>::value value;
};

// search in the next row
template <typename B, int R, CellType type>
struct FindCar<B, R, -1, type>
{
    typedef typename FindCar<B, R - 1, B::width - 1, type>::value value;
};
/********************** FindCar end **********************/
template <typename B, int R, int C>
struct CheckIfCanExit;

template <typename B, int R, int C>
struct CheckIfCanExit
{
    // find the cell
    typedef typename GetAtIndex<C, typename GetAtIndex<R, typename B::board>::value>::value curr_cell;

    static constexpr bool current_place_ok = (curr_cell::type == X || curr_cell::type == EMPTY);
    static constexpr bool reached_right_end = C + 1 >= B::width;

    typedef typename ConditionalAndValue<
        current_place_ok && reached_right_end, ValueWrapperBool<true>,
        ConditionalAndValue<!current_place_ok, ValueWrapperBool<false>,
                            CheckIfCanExit<B, R, C + 1>>>::value value;
};

/*template <typename B, int R, int C, int N>
struct CheckIfCanExit;

template <typename B, int R, int C, int N>
struct CheckIfCanExit
{
    // find the cell
    typedef typename GetAtIndex<C, typename GetAtIndex<R, typename B::board>::value>::value curr_cell;
    static constexpr bool current_place_ok = (curr_cell::type == X || curr_cell::type == EMPTY);
    // static constexpr bool reached_right_end = C + 1 >= B::width;
    //static constexpr bool value = ConditionalBool<(current_place_ok), CheckIfCanExit<B, R, C + 1, N - 1>::value, false>::value;
    static constexpr bool value = (curr_cell::type == X || curr_cell::type == EMPTY) ? (CheckIfCanExit<B, R, C + 1, N - 1>::value) : false;
};

template <typename B, int R, int C>
struct CheckIfCanExit<B, R, C, 0>
{
    static constexpr bool value = true; // The car can exit if no more steps are left
};*/
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
    //static constexpr bool result = CheckIfCanExit<B, location::row, location::col, (B::width - 1) - C>::value;
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