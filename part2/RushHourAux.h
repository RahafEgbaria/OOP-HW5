#ifndef PART2_RUSHHOUR_AUX_H
#define PART2_RUSHHOUR_AUX_H

// #include "Printer.h"
#include "MoveVehicle.h"
#include "List.h"
#include "Direction.h"
#include "Utilities.h"
#include "GameBoard.h"
#include "CellType.h"
#include "BoardCell.h"

using namespace std;

/********************** FindCar begin **********************/
// gets board, row, col, and car type
// returns the car in (R,C) with the type T
// note: the car SHOULD be found
template <typename B, int R, int C, CellType T>
struct FindCar;

template <typename B, int R, int C, CellType T>
struct FindCar
{
    // find the cell
    typedef typename GetAtIndex<C, typename GetAtIndex<R, typename B::board>::value>::value curr_cell;

    // check if this is the wanted car. If no, search in the prev col
    typedef typename NestedConditional<(curr_cell::type == T),
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
/********************** CheckIfCanExit begin **********************/
// gets board, row, and col
// checks if the car in (R,C) can exit the grid by moving straight to the right
template <typename B, int R, int C>
struct CheckIfCanExit;

template <typename B, int R, int C>
struct CheckIfCanExit
{
    // find the cell
    typedef typename GetAtIndex<C, typename GetAtIndex<R, typename B::board>::value>::value curr_cell;

    // check
    /*typedef typename NestedConditional<(curr_cell::type == X || curr_cell::type == EMPTY) && ((B::width - 1) - C > 0),
                                       CheckIfCanExit<B, R, C + 1>,
                                       NestedConditional<((B::width - 1) - C == 0),
                                                         NestedConditionalValueWrapper<NestedConditionalBoolWrapper<true>>,
                                                         NestedConditionalValueWrapper<NestedConditionalBoolWrapper<false>>>>::value value;*/

    typedef typename NestedConditional<((B::width - 1) - C == 0) && (curr_cell::type == X || curr_cell::type == EMPTY),
                                       NestedConditionalValueWrapper<NestedConditionalBoolWrapper<true>>,
                                       NestedConditional<(curr_cell::type == X || curr_cell::type == EMPTY),
                                                         CheckIfCanExit<B, R, C + 1>,
                                                         NestedConditionalValueWrapper<NestedConditionalBoolWrapper<false>>>>::value value;
};
/********************** CheckIfCanExit end **********************/

#endif // PART2_RUSHHOUR_AUX_H