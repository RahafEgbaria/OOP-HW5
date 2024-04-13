#ifndef PART2_MOVEVEHICLE_AUX_H
#define PART2_MOVEVEHICLE_AUX_H

#include "GameBoard.h"
#include "Direction.h"
#include "CellType.h"
#include "BoardCell.h"
#include "Utilities.h"

/********************** Location begin **********************/
template <int R, int C>
struct Location
{
    typedef Location<R, C> value;

    static constexpr int row = R;
    static constexpr int col = C;
};
/********************** Location end **********************/
/********************** UpdateCell begin **********************/
// gets board, row, col, and new cell
// updates the cel in (R, C) to new cell
template <typename B, int R, int C, typename Cell>
struct UpdateCell
{
    // find the row that includes the cell we want to update
    typedef typename GetAtIndex<R, typename B::board>::value row;

    // update row
    typedef typename SetAtIndex<C, Cell, row>::list tmp_row;

    // set the updated row
    typedef typename SetAtIndex<R, tmp_row, typename B::board>::list tmp_board;

    // updated Board!!
    typedef GameBoard<tmp_board> board;
};
/********************** UpdateCell end **********************/
/********************** GetNextCell begin **********************/
// gets board, row, col, and direction
// finds the next cell of the car in (R, C)
template <typename B, int R, int C, Direction D>
struct GetNextCell
{
    // find the cell
    typedef typename GetAtIndex<C, typename GetAtIndex<R, typename B::board>::value>::value curr_cell;

    // get the next cell
    static constexpr int tmp_row = ConditionalInteger<(D == UP || D == DOWN),
                                                      ConditionalInteger<D == UP, R - 1, R + 1>::value, R>::value;
    static constexpr int tmp_col = ConditionalInteger<(D == LEFT || D == RIGHT),
                                                      ConditionalInteger<D == LEFT, C - 1, C + 1>::value, C>::value;
    static constexpr int row = ConditionalInteger<(tmp_row < 0 || tmp_row > B::length - 1), R, tmp_row>::value;
    static constexpr int col = ConditionalInteger<(tmp_col < 0 || tmp_col > B::width - 1), C, tmp_col>::value;

    // get the next cell
    typedef typename GetAtIndex<col, typename GetAtIndex<row, typename B::board>::value>::value tmp_cell;

    // check if next cell is not empty
    static_assert(tmp_cell::type == curr_cell::type || tmp_cell::type == EMPTY, "ERROR: CAN'T MOVE VEHICLE, CELL IS NOT EMPTY!!");

    typedef typename Conditional<tmp_cell::type == curr_cell::type,
                                 GetNextCell<B, row, col, D>,
                                 Location<row, col>>::value value;
};
/********************** GetNextCell end **********************/
/********************** GetPrevCell begin **********************/
// gets board, row, col, and direction
// finds the cell that wiil be empty after moving the car in (R, C) one step
template <typename B, int R, int C, Direction D>
struct GetPrevCell
{
    // find curr cell
    typedef typename GetAtIndex<C, typename GetAtIndex<R, typename B::board>::value>::value curr_cell;
    // get the empty to be cell
    static constexpr int tmp_row = ConditionalInteger<(D == UP || D == DOWN),
                                                      ConditionalInteger<D == UP, R + 1, R - 1>::value, R>::value;
    static constexpr int tmp_col = ConditionalInteger<(D == LEFT || D == RIGHT),
                                                      ConditionalInteger<D == LEFT, C + 1, C - 1>::value, C>::value;

    static constexpr int row = ConditionalInteger<(tmp_row < 0 || tmp_row > B::length - 1), R, tmp_row>::value;
    static constexpr int col = ConditionalInteger<(tmp_col < 0 || tmp_col > B::width - 1), C, tmp_col>::value;

    // get the cell
    typedef typename GetAtIndex<col, typename GetAtIndex<row, typename B::board>::value>::value tmp_cell;

    typedef typename NestedConditional<(row == R && col == C) || (tmp_cell::type != curr_cell::type),
                                       Location<R, C>,
                                       GetPrevCell<B, row, col, D>>::value value;

    /*typedef typename Conditional<(row == R && col == C) || (tmp_cell::type != curr_cell::type),
                                 Location<R, C>,
                                 GetPrevCell<B, row, col, D>>::value value;*/
};
/********************** GetPrevCell end **********************/
/********************** TakeOneStep begin **********************/
// gets board, row, col, and direction
// finds the cell of the car in (R, C) and takes one step
template <typename B, int R, int C, Direction D>
struct TakeOneStep
{
    // get curr cell (the cell that is going to be empty after the move)
    typedef typename GetAtIndex<C, typename GetAtIndex<R, typename B::board>::value>::value curr_cell;

    // get the next cell
    typedef typename GetNextCell<B, R, C, D>::value next_cell;

    // get the empty-to-be cell
    typedef typename GetPrevCell<B, R, C, D>::value empty_to_be;

    typedef typename UpdateCell<B, next_cell::row, next_cell::col, curr_cell>::board temp_board;
    typedef typename UpdateCell<temp_board, empty_to_be::row, empty_to_be::col, BoardCell<EMPTY, UP, 0>>::board board;
    // typedef typename UpdateCell<temp_board, R, C, BoardCell<EMPTY, UP, 0>>::board board;
};
/********************** TakeOneStep end **********************/
/********************** Execute begin **********************/
// gets board, row, col, direction, and A
// finds the cell of the car in (R, C) and takes A step
template <typename B, int R, int C, Direction D, int A>
struct Execute
{
    // get the next cell
    typedef typename GetNextCell<B, R, C, D>::value next_cell;
    // get the empty-to-be cell
    // typedef typename GetPrevCell<B, R, C, D>::value empty_to_be;
    // take one step
    typedef typename TakeOneStep<B, R, C, D>::board tmp_board;
    // typedef typename TakeOneStep<B, empty_to_be::row, empty_to_be::col, D>::board tmp_board;
    //  recursion
    typedef typename Execute<tmp_board, next_cell::row, next_cell::col, D, A - 1>::board board;
};

template <typename B, int R, int C, Direction D>
struct Execute<B, R, C, D, 0> // no steps left
{
    typedef B board;
};
/********************** Execute end **********************/

#endif // PART2_MOVEVEHICLE_AUX_H