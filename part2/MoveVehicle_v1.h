#ifndef PART2_MOVEVEHICLE_H
#define PART2_MOVEVEHICLE_H

#include "CellType.h"
#include "Direction.h"
#include "GameBoard.h"
#include "Utilities.h"
#include "BoardCell.h"

/********************** Location begin **********************/
template <int R, int C>
struct Location
{
    static constexpr int row = R;
    static constexpr int col = C;
};
/********************** Location end **********************/
/********************** Move begin **********************/
template <CellType T, Direction D, int A>
struct Move
{
    // check if cell is empty
    static_assert(T != EMPTY, "ERROR: CELL IS EMPTY!!");

    static constexpr CellType type = T;
    static constexpr Direction direction = D;
    static constexpr int amount = A;
};
/********************** Move begin **********************/
/********************** MoveVehicle begin **********************/
template <typename B, int R, int C, Direction D>
struct CarNextCell
{
    // find the cell
    typedef typename GetAtIndex<C, typename GetAtIndex<R, typename B::board>::value>::value curr_cell;

    // get the next cell
    static constexpr int row = ConditionalInteger<(D == UP || D == DOWN),
                                                  ConditionalInteger<D == UP, R - 1, R + 1>::value, R>::value;
    static constexpr int col = ConditionalInteger<(D == LEFT || D == RIGHT),
                                                  ConditionalInteger<D == LEFT, C - 1, C + 1>::value, C>::value;

    // get the next cell
    typedef typename GetAtIndex<col, typename GetAtIndex<row, typename B::board>::value>::value tmp_cell;

    static_assert(row >= 0 && row < B::length, "ERROR: CarNextCell -> ROW INVALID");
    static_assert(col >= 0 && col < B::width, "ERROR: CarNextCell -> COL INVALID");

    // check if next cell is not empty
    static_assert(tmp_cell::type == curr_cell::type || tmp_cell::type == EMPTY, "ERROR: CAN'T MOVE VEHICLE, CELL IS NOT EMPTY!!");

    // check if this is not a part of the car
    typedef typename Conditional<tmp_cell::type == curr_cell::type,
                                 typename CarNextCell<B, row, col, D>::value,
                                 Location<row, col>>::value value;
};
/**************************/
template <typename B, int R, int C, Direction D>
struct CarPrevCell
{
    // find curr cell
    typedef typename GetAtIndex<C, typename GetAtIndex<R, typename B::board>::value>::value curr_cell;

    // get the empty to be cell
    static constexpr int row = ConditionalInteger<(D == UP || D == DOWN),
                                                  ConditionalInteger<D == UP, R + 1, R - 1>::value, R>::value;
    static constexpr int col = ConditionalInteger<(D == LEFT || D == RIGHT),
                                                  ConditionalInteger<D == LEFT, C + 1, C - 1>::value, C>::value;

    // get the cell
    typedef typename GetAtIndex<col, typename GetAtIndex<row, typename B::board>::value>::value tmp_cell;

    // check if this is not a part of the car
    typedef typename Conditional<(tmp_cell::type == curr_cell::type),
                                 typename CarPrevCell<B, row, col, D>::value,
                                 Location<R, C>>::value value;
};
/**************************/
template <typename B, int R, int C, Direction D>
struct GetNextCell
{
    // find curr cell
    typedef typename GetAtIndex<C, typename GetAtIndex<R, typename B::board>::value>::value curr_cell;

    // get the next to be cell
    static constexpr int row = ConditionalInteger<(D == UP || D == DOWN),
                                                  ConditionalInteger<D == UP, R - 1, R + 1>::value, R>::value;
    static constexpr int col = ConditionalInteger<(D == LEFT || D == RIGHT),
                                                  ConditionalInteger<D == LEFT, C + 1, C - 1>::value, C>::value;

    // get the cell
    typedef typename GetAtIndex<col, typename GetAtIndex<row, typename B::board>::value>::value next_cell;
};
/**************************/
template <typename B, int R, int C, typename Cell = BoardCell<EMPTY, RIGHT, 0>>
struct UpdateCell
{
    // find the row that includes the cell we want to update
    typedef typename GetAtIndex<R, typename B::board>::value row;

    // update cell
    typedef typename SetAtIndex<C, Cell, row>::list tmp_row;

    // set the updated row
    typedef typename SetAtIndex<R, tmp_row, typename B::board>::list tmp_board;

    // updated Board!!
    typedef typename GameBoard<tmp_board>::board board;
};
/**************************/
template <typename B, int R, int C, Direction D>
struct TakeOneStep
{
    // get the curr cell
    typedef typename GetAtIndex<C, typename GetAtIndex<R, typename B::board>::value>::value curr_cell;

    // get the cell we want to move to next
    typedef typename GetNextCell<B, R, C, D>::value next_cell;
    // get the dimensions of the cell
    static constexpr int next_row = next_cell::row;
    static constexpr int next_col = next_cell::col;

    // update next cell
    typedef typename UpdateCell<B, next_row, next_col, curr_cell>::board tmp;
    // empty the cell on the back
    typedef typename UpdateCell<B, curr_cell::row, curr_cell::col>::board board;
};
/**************************/
template <typename B, int R, int C, Direction D, int A>
struct Execute
{
    // get the cell we want to move to next
    typedef GetNextCell<B, R, C, D> next_cell;
    // get the dimensions of the cell
    static constexpr int next_row = next_cell::row;
    static constexpr int next_col = next_cell::col;

    // check if nbext cell is valid
    static_assert(next_row >= 0 && next_row < B::length, "ERROR: NEXT CELL INVALID ROW!!");
    static_assert(next_col >= 0 && next_col < B::width, "ERROR: NEXT CELL INVALID COL!!");

    // continue
    typedef typename Execute<typename TakeOneStep<B, R, C, D>::board,
                             next_row, next_col, D, A - 1>::board board;
};

template <typename B, int R, int C, Direction D>
struct Execute<B, R, C, D, 0>
{
    typedef B board;
};
/**************************/

template <typename B, int R, int C, Direction D, int A>
struct MoveVehicle
{
    // check if input is valid
    static_assert(R >= 0 && R < B::length, "ERROR: INVALID ROW!!");
    static_assert(C >= 0 && C < B::width, "ERROR: INVALID COL!!");

    // find the cell
    typedef typename GetAtIndex<C, typename GetAtIndex<R, typename B::board>::value>::value curr_cell;
    // check if empty
    static_assert(curr_cell::type != EMPTY, "ERROR: CELL IS EMPTY!!");
    // check if can go that direction
    static_assert(curr_cell::direction == D, "ERROR: CAN'T MOVE THIS DIRECTION!!");

    // get the cell we want to move
    typedef typename CarPrevCell<B, R, C, D>::value cell;
    // UPDATE THE BOARD
    typedef typename Execute<B, cell::row, cell::col, D, A>::board board;
};
/********************** MoveVehicle end **********************/

#endif // PART2_MOVEVEHICLE_H