#ifndef PART2_WORK_MOVEVEHICLE_H
#define PART2_WORK_MOVEVEHICLE_H

#include "Direction.h"
#include "CellType.h"
#include "GameBoard.h"
#include "Utilities.h"
#include "BoardCell.h"

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
/********************** Move end **********************/
/********************** Location begin **********************/
template <int R, int C>
struct Location
{
    static constexpr int row = R;
    static constexpr int col = C;
    typedef Location<row, col> value;
};
/********************** Location end **********************/
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

    static constexpr bool found_back =
        (tmp_cell::type != curr_cell::type) ||
        (row == R && col == C);
    typedef typename ConditionalAndValue<found_back, Location<R, C>,
                                         GetPrevCell<B, row, col, D>>::value value;
};
/**************************/
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
/**************************/

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
/**************************/
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
/********************** MoveVehicle begin **********************/
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
    static_assert((D <= 1 && curr_cell::direction <= 1) || (D > 1 && curr_cell::direction > 1), "ERROR: CAN'T MOVE THIS DIRECTION!!");

    // Execute
    typedef typename Execute<B, R, C, D, A>::board board;
};
/********************** MoveVehicle END **********************/

#endif // PART2_WORK_MOVEVEHICLE_H