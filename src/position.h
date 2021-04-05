#pragma once

#include <iostream>
#include <vector>
#include "types.h"
#include "util.h"

namespace gelato {

class Position {
public:
    Position();
    Position(std::string fen);

    // piece bitboards
    U64 bitboards[12] = {C64(0)};
    U64 occupancies[3] = {C64(0)};
    
    Side side = Side::White;
    Square enpassant = Square::None;

    Castle castle;

    std::string fen;

    friend std::ostream &operator<<(std::ostream &stream, const Position position);
};
} // namespace gelato