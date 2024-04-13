#ifndef PART2_BOARDCELL_H
#define PART2_BOARDCELL_H

#include "CellType.h"
#include "Direction.h"

/********************** BoardCell begin **********************/
template<CellType T, Direction D, int L>
struct BoardCell {
    static constexpr CellType type = T;
    static constexpr Direction direction = D;
    static constexpr int length = L;
};

/*struct EmptyCell {
    static constexpr CellType type = EMPTY;
};*/
/********************** BoardCell begin **********************/

#endif //PART2_BOARDCELL_H