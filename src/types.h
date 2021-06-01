#pragma once 

namespace gelato {

// bitboard macros
#define C64(constantU64) constantU64##ULL

// some fens
#define FEN_EMPTY_BOARD "8/8/8/8/8/8/8/8 w - - "
#define FEN_START_POSITION "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 "
#define FEN_TRICKY_POSITION "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1 "
#define FEN_KILLER_POSITION "rnbqkb1r/pp1p1pPp/8/2p1pP2/1P1P4/3P3P/P1P1P3/RNBQKBNR w KQkq d6 0 1"
#define FEN_CMK_POSITION "r2q1rk1/ppp2ppp/2n1bn2/2b1p3/3pP3/3P1NPP/PPP1NPB1/R1BQ1RK1 b - - 0 9 "
#define FEN_PAPA "6k1/5pp1/R2p2qp/3P2r1/4Q3/2N4P/PP3PP1/3b2K1 b - - 2 29 "

// bitboard data type
typedef unsigned long long U64;

// bitboard macros
#define C64(constantU64) constantU64##ULL

enum PieceType {
    Pawn, Knight, Bishop, Rook, King, Queen
};

enum Piece {
    P, N, B, R, Q, K, p, n, b, r, q, k
};

enum Square: int {
    A1, B1, C1, D1, E1, F1, G1, H1,
    A2, B2, C2, D2, E2, F2, G2, H2, 
    A3, B3, C3, D3, E3, F3, G3, H3, 
    A4, B4, C4, D4, E4, F4, G4, H4, 
    A5, B5, C5, D5, E5, F5, G5, H5, 
    A6, B6, C6, D6, E6, F6, G6, H6, 
    A7, B7, C7, D7, E7, F7, G7, H7, 
    A8, B8, C8, D8, E8, F8, G8, H8, 
    None
};

enum Side { White, Black, Both };

/* 
    0001 (1): white king can castle to the king side
    0010 (2): white king can castle to the queen side
    0100 (4): black king can castle to the king side
    1000 (8): black king can castle to the queen side
*/

enum Castle: int {
    Wks = 1, Wqs = 2,
    Bks = 4, Bqs = 8,
};

// avoid conflicts
enum File: int {
    File1, File2, File3, File4, File5, File6, File7, File8
};

enum Rank: int {
    Rank1, Rank2, Rank3, Rank4, Rank5, Rank6, Rank7, Rank8
};

enum Fen {
    PiecePlacement,
    SideToMove,
    CastlingAbility,
    EnPassantTargetSquare,
    HalfmoveClock,
    FullmoveCounter
};

constexpr File GetFile(Square square) {
    return File(square & 7);
}

constexpr Rank GetRank(Square square) {
    return Rank(square >> 3);
}

constexpr Square GetSquare(Rank rank, File file) {
    return Square((rank << 3) + file);
}

} // namespace gelato