#ifndef PART2_MOVEVEHICLE_H
#define PART2_MOVEVEHICLE_H

#include "MoveVehicleAux.h"

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

#endif // PART2_MOVEVEHICLE_H