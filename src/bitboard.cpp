#include "bitboard.h"

namespace gelato {

namespace bitboard {

U64 GetBit(U64 bitboard, Square square) {
    return bitboard & (C64(1) << square);
}
void SetBit(U64 &bitboard, Square square) {
    bitboard |= (C64(1) << square);
}
void PopBit(U64 &bitboard, Square square) {
    bitboard &= ~(C64(1) << square);

}

std::string Pretty(U64 bitboard) {
    Square square;
    std::string s = "+---+---+---+---+---+---+---+---+\n";
    for (int rank = Rank::Rank8; rank >= Rank::Rank1; rank--) {
        for (int file = File::File1; file <= File::File8; file++) {
            square = GetSquare(Rank(rank), File(file));
            s += bitboard & GetBit(bitboard, square) ? "| X " : "|   ";
        }
        s += "| " + std::to_string(rank + 1) + "\n+---+---+---+---+---+---+---+---+\n";
    }
    s += "  a   b   c   d   e   f   g   h\n";

    return s;
}

} // namespace bitboard

} // namespace gelato
