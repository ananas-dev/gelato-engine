#pragma once

#include <string>
#include "types.h"

namespace gelato {

namespace bitboard {

// bit operations
U64 GetBit(U64 bitboard, Square square);
void SetBit(U64 &bitboard, Square square);
void PopBit(U64 &bitboard, Square square);

std::string Pretty(U64 bitboard);

} // namespace bitboard

} // namespace gelato