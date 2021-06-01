#pragma once

#include <iostream>
#include <vector>
#include <regex>
#include <string>
#include <array>
#include "types.h"
#include "util.h"
#include "bitboard.h"

namespace gelato {

class Position {
public:
    Position();
    Position(std::string fen);

    // piece bitboards
    std::array<U64, 12> bitboards = {C64(0)};
    std::array<U64, 12> occupancies = {C64(0)};
    
    Side side = Side::White;
    Square enpassant = Square::None;

    // no castle by default
    Castle castle = Castle(0b0000);

    std::string fen = "";

    void Clear();
    void LoadFen(std::string fen);

    friend std::ostream &operator<<(std::ostream &stream, const Position position);

private:
    std::string asciiPieces = "PNBRQKpnbrqk";
};
} // namespace gelato