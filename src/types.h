#pragma once 

namespace gelato {
    // bitboard macros
    #define C64(constantU64) constantU64##ULL
    #define get_bit(bitboard, square) ((bitboard) & (C64(1) << (square)))
    #define set_bit(bitboard, square) ((bitboard) |= (C64(1) << (square)))
    #define pop_bit(bitboard, square) ((bitboard) &= ~(C64(1) << (square)))

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

    enum Square {
        A8, B8, C8, D8, E8, F8, G8, H8, 
        A7, B7, C7, D7, E7, F7, G7, H7, 
        A6, B6, C6, D6, E6, F6, G6, H6, 
        A5, B5, C5, D5, E5, F5, G5, H5, 
        A4, B4, C4, D4, E4, F4, G4, H4, 
        A3, B3, C3, D3, E3, F3, G3, H3, 
        A2, B2, C2, D2, E2, F2, G2, H2, 
        A1, B1, C1, D1, E1, F1, G1, H1,
        None
    };

    enum Side { White, Black, Both };

    /* 
       0001 (1): white king can castle to the king side
       0010 (2): white king can castle to the queen side
       0100 (4): black king can castle to the king side
       1000 (8): black king can castle to the queen side
    */

    enum Castle {
        Wks = 1, Wqs = 2,
        Bks = 4, Bqs = 8
    };

    enum sliding_piece { rook, bishop };

    enum File {
        File_1, File_2, File_3, File_4, File_5, File_6, File_7, File_8
    };

    enum Rank {
        Rank_1, Rank_2, Rank_3, Rank_4, Rank_5, Rank_6, Rank_7, Rank_8
    };

    constexpr File GetFile(Square s) {
        return File(s & 7);
    }

    constexpr Rank GetRank(Square s) {
        return Rank(s >> 8);
    }
} // namespace gelato