#ifndef CODE_MOVEVEHICLE_H
#define CODE_MOVEVEHICLE_H

#include "CellType.h"
#include "Direction.h"
#include "GameBoard.h"
#include "Utilities.h"
#include <iostream>
#include "Printer.h"


using namespace std;

// Find_Car_Helper Class Declaration
// This class recursively iterates over the board's cells and finds the coordinates of a cell containing an end (the front or back) of the car "type" (note that the first end to be found depends on the direction of the search).
// type - the car to find
// type2 - the car on the current cell
// (row, col) - the coordinates of the current cell
// done - search is done
// B - main list of the board
template <CellType type, CellType type2, int row, int col, bool done, typename B>
struct Find_Car_Helper{
    typedef typename GameBoard<B>::board mainList;
    static constexpr bool last_row = (mainList::size == row + 1);

    static constexpr bool found = /*/ COMPLETE /*/;
    static constexpr bool last_cell_in_board = /*/ COMPLETE /*/;

    static_assert(!(!found && last_cell_in_board), "Type was not found!");

    static constexpr int next_row = /*/ COMPLETE using ConditionalInteger /*/; // this is the next cell's row
    static constexpr int next_col = /*/ COMPLETE using ConditionalInteger /*/; // this is the next cell's column

    typedef typename /*/ COMPLETE using GetAtIndex /*/ next_row_list;
    typedef typename /*/ COMPLETE using GetAtIndex /*/ next_cell;
    typedef /*/ COMPLETE using Find_Car_Helper (recursive call) /*/ next_helper;

    static constexpr int X_row = ConditionalInteger<found, row, next_helper::X_row >::value;
    static constexpr int X_col = ConditionalInteger<found, col, next_helper::X_col >::value;
};

// Find_Car_Helper Specialization - stopping condition
template </*/COMPLETE/*/>
struct Find_Car_Helper</*/COMPLETE/*/>{
    /*/COMPLETE/*/
};

// FindCar Class Declaration
// (uses Find_Car_Helper to find car "type" in board "Bo" - read Find_Car_Helper for more info)
template<CellType type, typename Bo>
struct FindCar{
    typedef Bo game_board;
    typedef typename game_board::board mainList;
    static constexpr int last_col_idx = mainList::head::size - 1;

    typedef typename /*/COMPLETE/*/ first_cell;

    typedef Find_Car_Helper<type,first_cell::type,0,last_col_idx,false,mainList> car_loc;
    static constexpr int X_row_idx = car_loc::X_row;
    static constexpr int X_col_idx = car_loc::X_col;
};

// Dir Class Declaration
// This class computes the further end of a car respect to "c" given the end found using FindCar.
// (e.g. if "c"=RIGHT, the further end of the car is the left end...)
// c - a direction
// (Ro, Col) - a random end of the car
// len - the car's length
template<Direction c,int Ro,int Col,int len>
struct Dir{};

// Dir Specialization (you need to implement more specializations similarly)
template< int Ro,int Col,int len>
struct Dir<RIGHT,Ro,Col,len> {
    static constexpr int row_i=/*/COMPLETE/*/;
    static constexpr int col_i=/*/COMPLETE/*/;
};


// direct Class Declaration
// This class recursively moves a car "Am" steps on the board in a certain direction
// d - the direction of the movement
// counter - recursive counter (remaining amount of steps)
// myL - main list of the board
// my_cell - a cell on the current board containing the car to be moved
// (Ro, Co) - coordinates of the further end of the car respect to "d" (e.g. if "d"=RIGHT, the further end of the car is the left end...)
template<Direction d,int counter,typename myL,typename my_cell,int Co,int Ro>
struct direct{};

// direct Specialization (you need to implement more specializations similarly)
template<int counter,typename myL,typename my_cell,int Co,int Ro>
struct direct<RIGHT,counter,myL,my_cell,Co, Ro>{
    typedef typename /*/ COMPLETE using direct (recursive call) /*/ mainList; // main list of the board after we moved the car "count"-1 steps
    typedef GetAtIndex<Ro,mainList> subList ;
    typedef typename /*/COMPLETE/*/ celli;  // this is the closer end (respect to "d") after the #"count" step
    typedef typename /*/COMPLETE/*/ to_celli; // this is the further end (respect to "d") before the #"count" step (after the #("count"-1) step)
    static_assert(/*/COMPLETE/*/, "Error,Collision cell MoveVehicle");
    typedef typename /*/COMPLETE/*/ first;
    typedef typename /*/COMPLETE (use first)/*/ second;
    typedef typename SetAtIndex<Ro,second,mainList>::list LL;
    typedef LL moved;
};

// direct Specialization (you need to implement more specializations similarly)
template<typename myL,typename my_cell,int Co,int Ro>
struct direct<RIGHT,0,myL,my_cell,Co, Ro> {
    /*/COMPLETE/*/;
};
/********************** MoveVehicle begin **********************/
// MoveVehicle Class Declaration
template<typename gameBoard, int R, int C, Direction D, int A>
struct MoveVehicle{};

// MoveVehicle Specialization
template<typename B, int R, int C, Direction D, int A>
struct MoveVehicle<B, R, C, D, A>{
    static_assert(R >= 0 && R < B::length, "Error Row,Move");
    static_assert(C >= 0 && C < B::width, "Error column,Move");

    //typedef GameBoard<B> PrevBoard;
    typedef typename B::board mainList;
    typedef GetAtIndex<R1,mainList> subList;
    typedef GetAtIndex<C1,typename subList::value> cell;
    typedef typename cell::value my_cell;

    static_assert(my_cell::type != EMPTY, "Error,empty cell MoveVehicle");
    static_assert(my_cell::direction == D, "Error,direction cell MoveVehicle");

    static constexpr int R2= FindCar<my_cell::type,PrevBoard>::X_row_idx;
    static constexpr int C2= FindCar<my_cell::type,PrevBoard>::X_col_idx;
    // the further end:
    static constexpr int R3= Dir<Dl, R2, C2, my_cell::length>::row_i;
    static constexpr int C3= Dir<Dl, R2, C2, my_cell::length>::col_i;

    typedef typename direct<Dl, A, B, my_cell, C3, R3>::moved o1;
    typedef GameBoard<o1> board;
};
/********************** MoveVehicle end **********************/
#endif //CODE_MOVEVEHICLE_H
