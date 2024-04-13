#ifndef PART2_MOVEVEHICLE_AUX_H
#define PART2_MOVEVEHICLE_AUX_H

#include "Direction.h"
#include "CellType.h"
#include "GameBoard.h"
#include "Utilities.h"
#include "BoardCell.h"

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
        static constexpr int row = ConditionalInteger<(D == UP || D == DOWN),
                                                      ConditionalInteger<D == UP, R - 1, R + 1>::value, R>::value;
        static constexpr int col = ConditionalInteger<(D == LEFT || D == RIGHT),
                                                      ConditionalInteger<D == LEFT, C - 1, C + 1>::value, C>::value;

        // get the next cell
        typedef typename GetAtIndex<col, typename GetAtIndex<row, typename B::board>::value>::value tmp_cell;

        // check if tmp cell is valid
        static_assert(row >= 0 && row < B::length, "ERROR: ROW INVALID");
        static_assert(col >= 0 && col < B::width, "ERROR: COL INVALID");

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
        static constexpr int row_before_fix = ConditionalInteger<(D == UP || D == DOWN),
                                                                 ConditionalInteger<D == UP, R + 1, R - 1>::value, R>::value;
        static constexpr int col_before_fix = ConditionalInteger<(D == LEFT || D == RIGHT),
                                                                 ConditionalInteger<D == LEFT, C + 1, C - 1>::value, C>::value;
        static constexpr int row =
            row_before_fix < 0 || row_before_fix >= B::length ? R : row_before_fix;
        static constexpr int col =
            col_before_fix < 0 || col_before_fix >= B::width ? C : col_before_fix;
        // check if tmp cell is valid
        static_assert(row >= 0 && row < B::length, "ERROR: ROW INVALID");
        static_assert(col >= 0 && col < B::width, "ERROR: COL INVALID");

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
// assume that the curr cell is the cell that is going to be empty after the move
template <typename B, int R, int C, Direction D>
struct TakeOneStep
{
        // get curr cell (the cell that is going to be empty after the move)
        typedef typename GetAtIndex<C, typename GetAtIndex<R, typename B::board>::value>::value curr_cell;
        // get the next cell
        typedef typename GetNextCell<B, R, C, D>::value next_cell;
        // update cells
        typedef typename UpdateCell<B, next_cell::row, next_cell::col, curr_cell>::board temp_board;
        typedef typename UpdateCell<temp_board, R, C, BoardCell<EMPTY, UP, 0>>::board board;
};
/**************************/
template <typename B, int R, int C, Direction D, int A>
struct Execute
{
        // get the next cell
        typedef typename GetNextCell<B, R, C, D>::value next_cell;
        // get the empty-to-be cell
        typedef typename GetPrevCell<B, R, C, D>::value empty_to_be;
        // take one step
        typedef typename TakeOneStep<B, empty_to_be::row, empty_to_be::col, D>::board tmp_board;
        // recursion
        typedef typename Execute<tmp_board, next_cell::row, next_cell::col, D, A - 1>::board board;
};

template <typename B, int R, int C, Direction D>
struct Execute<B, R, C, D, 0> // no steps left
{
        typedef B board;
};

#endif // PART2_MOVEVEHICLE_AUX_H