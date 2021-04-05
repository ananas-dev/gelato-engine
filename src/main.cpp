#include <iostream>
#include <cstring>
#include <bit>
#include <vector>

#include "types.h"
#include "position.h"
#include "util.h"

namespace gelato {


///////////
// DEBUG //
///////////

namespace bitboard {

    std::string Pretty(U64 bitboard) {
        std::string s = "  +---+---+---+---+---+---+---+---+\n";
        for (int rank = 0; rank < 8; rank++) {
            s += std::to_string(8 - rank) + " ";
            for (int file = 0; file < 8; file++) {
                // convert file & rank into square index
                int square = rank * 8 + file;
                s += bitboard & get_bit(bitboard, square) ? "| X " : "|   ";
            }

            s += "| \n  +---+---+---+---+---+---+---+---+\n";
        }

        s += "    a   b   c   d   e   f   g   h\n";

        return s;

    }
}


//////////
// BITS //
//////////

static inline int GetLsfbIndex(U64 bitboard) {
    // make sure bitboard is not 0
    if (bitboard) {
        return std::popcount((bitboard & -bitboard) - 1);
    } else {
        return -1;
    }
}

///////////////////
// RANDOM NUMBERS //
///////////////////

// pseudo random number state;
unsigned int state = 1804289383;

// generate 32-bit pseudo legal numbers
unsigned int GetRandomU32Number()
{
    // get current state
    unsigned int number = state;

    // XOR shift algorithm
    number ^= number << 13;
    number ^= number >> 17;
    number ^= number << 5;

    // update random number state
    state = number;

    return state;
}

// gen 64-bit pseudo legal numbers
U64 GetRandomU64Number() {
    //define 4 random numbers
    U64 n1, n2, n3, n4;

    // init random numbers slincing 16 bits from MS1B side
    n1 = (U64)(GetRandomU32Number() & 0xFFFF);
    n2 = (U64)(GetRandomU32Number() & 0xFFFF);
    n3 = (U64)(GetRandomU32Number() & 0xFFFF);
    n4 = (U64)(GetRandomU32Number() & 0xFFFF);

    return n1 | (n2 << 16) | (n3 << 32) | (n4 << 48);
}

// gen magic number candidate
U64 GenMagicNumber() {
    return GetRandomU64Number() & GetRandomU64Number() & GetRandomU64Number();
}


/////////////
// ATTACKS //
/////////////

const U64 not_a_file = C64(18374403900871474942);
const U64 not_h_file = C64(9187201950435737471);
const U64 not_hg_file = C64(4557430888798830399);
const U64 not_ab_file = C64(18229723555195321596);

// relevant occupancy for bit count of every square

const int bishop_relevant_bits[64] = {
    6, 5, 5, 5, 5, 5, 5, 6,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    6, 5, 5, 5, 5, 5, 5, 6
};

const int rook_relevant_bits[64] = {
    12, 11, 11, 11, 11, 11, 11, 12,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    12, 11, 11, 11, 11, 11, 11, 12
};

const U64 bishop_magics[64] = {
    C64(0x0002020202020200), C64(0x0002020202020000), C64(0x0004010202000000), C64(0x0004040080000000),
    C64(0x0001104000000000), C64(0x0000821040000000), C64(0x0000410410400000), C64(0x0000104104104000),
    C64(0x0000040404040400), C64(0x0000020202020200), C64(0x0000040102020000), C64(0x0000040400800000),
    C64(0x0000011040000000), C64(0x0000008210400000), C64(0x0000004104104000), C64(0x0000002082082000),
    C64(0x0004000808080800), C64(0x0002000404040400), C64(0x0001000202020200), C64(0x0000800802004000),
    C64(0x0000800400A00000), C64(0x0000200100884000), C64(0x0000400082082000), C64(0x0000200041041000),
    C64(0x0002080010101000), C64(0x0001040008080800), C64(0x0000208004010400), C64(0x0000404004010200),
    C64(0x0000840000802000), C64(0x0000404002011000), C64(0x0000808001041000), C64(0x0000404000820800),
    C64(0x0001041000202000), C64(0x0000820800101000), C64(0x0000104400080800), C64(0x0000020080080080),
    C64(0x0000404040040100), C64(0x0000808100020100), C64(0x0001010100020800), C64(0x0000808080010400),
    C64(0x0000820820004000), C64(0x0000410410002000), C64(0x0000082088001000), C64(0x0000002011000800),
    C64(0x0000080100400400), C64(0x0001010101000200), C64(0x0002020202000400), C64(0x0001010101000200),
    C64(0x0000410410400000), C64(0x0000208208200000), C64(0x0000002084100000), C64(0x0000000020880000),
    C64(0x0000001002020000), C64(0x0000040408020000), C64(0x0004040404040000), C64(0x0002020202020000),
    C64(0x0000104104104000), C64(0x0000002082082000), C64(0x0000000020841000), C64(0x0000000000208800),
    C64(0x0000000010020200), C64(0x0000000404080200), C64(0x0000040404040400), C64(0x0002020202020200)
};

const U64 rook_magics[64] = {
    C64(0x0080001020400080), C64(0x0040001000200040), C64(0x0080081000200080), C64(0x0080040800100080),
    C64(0x0080020400080080), C64(0x0080010200040080), C64(0x0080008001000200), C64(0x0080002040800100),
    C64(0x0000800020400080), C64(0x0000400020005000), C64(0x0000801000200080), C64(0x0000800800100080),
    C64(0x0000800400080080), C64(0x0000800200040080), C64(0x0000800100020080), C64(0x0000800040800100),
    C64(0x0000208000400080), C64(0x0000404000201000), C64(0x0000808010002000), C64(0x0000808008001000),
    C64(0x0000808004000800), C64(0x0000808002000400), C64(0x0000010100020004), C64(0x0000020000408104),
    C64(0x0000208080004000), C64(0x0000200040005000), C64(0x0000100080200080), C64(0x0000080080100080),
    C64(0x0000040080080080), C64(0x0000020080040080), C64(0x0000010080800200), C64(0x0000800080004100),
    C64(0x0000204000800080), C64(0x0000200040401000), C64(0x0000100080802000), C64(0x0000080080801000),
    C64(0x0000040080800800), C64(0x0000020080800400), C64(0x0000020001010004), C64(0x0000800040800100),
    C64(0x0000204000808000), C64(0x0000200040008080), C64(0x0000100020008080), C64(0x0000080010008080),
    C64(0x0000040008008080), C64(0x0000020004008080), C64(0x0000010002008080), C64(0x0000004081020004),
    C64(0x0000204000800080), C64(0x0000200040008080), C64(0x0000100020008080), C64(0x0000080010008080),
    C64(0x0000040008008080), C64(0x0000020004008080), C64(0x0000800100020080), C64(0x0000800041000080),
    C64(0x00FFFCDDFCED714A), C64(0x007FFCDDFCED714A), C64(0x003FFFCDFFD88096), C64(0x0000040810002101),
    C64(0x0001000204080011), C64(0x0001000204000801), C64(0x0001000082000401), C64(0x0001FFFAABFAD1A2)
};

// pawn attacks table [side][square]
U64 pawn_attacks[2][64];

// knight attacks table [square]
U64 knight_attacks[64];

// king attack table
U64 king_attacks[64];

// bishop attack masks
U64 bishop_masks[64];

// rook attack masks
U64 rook_masks[64];

// bishop attack table [square][occupancies]
U64 bishop_attacks[64][512];

// rook attacks table [square][occupancies]
U64 rook_attacks[64][4096];

// generate pawn attacks
U64 MaskPawnAttacks(Side side, Square square) {
    U64 attacks = C64(0);
    U64 bitboard = C64(0);

    set_bit(bitboard, square);

    if (side == Side::White) {
        if ((bitboard >> 7) & not_a_file)
            attacks |= (bitboard >> 7);
        if ((bitboard >> 9) & not_h_file)
            attacks |= (bitboard >> 9);
    } else if(side == Side::Black) {
        if ((bitboard << 7) & not_h_file)
            attacks |= (bitboard << 7);
        if ((bitboard << 9) & not_a_file)
            attacks |= (bitboard << 9);
    }

    return attacks;
}

// generate knight attacks
U64 MaskKnightAttacks(Square square) {
    U64 attacks = C64(0);
    U64 bitboard = C64(0);

    set_bit(bitboard, square);

    if ((bitboard >> 17) & not_h_file)
        attacks |= (bitboard >> 17);
    if ((bitboard >> 15) & not_a_file)
        attacks |= (bitboard >> 15);
    if ((bitboard >> 10) & not_hg_file)
        attacks |= (bitboard >> 10);
    if ((bitboard >> 6) & not_ab_file)
        attacks |= (bitboard >> 6);

    if ((bitboard << 17) & not_a_file)
        attacks |= (bitboard << 17);
    if ((bitboard << 15) & not_h_file)
        attacks |= (bitboard << 15);
    if ((bitboard << 10) & not_ab_file)
        attacks |= (bitboard << 10);
    if ((bitboard << 6) & not_hg_file)
        attacks |= (bitboard << 6);

    return attacks;
}

// generate king attacks
U64 MaskKingAttacks(Square square) {
    U64 attacks = C64(0);
    U64 bitboard = C64(0);

    set_bit(bitboard, square);

    if (bitboard >> 8)
        attacks |= (bitboard >> 8);
    if ((bitboard >> 9) & not_h_file)
        attacks |= (bitboard >> 9);
    if ((bitboard >> 7) & not_a_file)
        attacks |= (bitboard >> 7);
    if ((bitboard >> 1) & not_h_file)
        attacks |= (bitboard >> 1);

    if (bitboard << 8)
        attacks |= (bitboard << 8);
    if ((bitboard << 9) & not_a_file)
        attacks |= (bitboard << 9);
    if ((bitboard << 7) & not_h_file)
        attacks |= (bitboard << 7);
    if ((bitboard << 1) & not_a_file)
        attacks |= (bitboard << 1);


    return attacks;
}

U64 MaskBishopAttacks(Square square) {
    U64 attacks = C64(0);

    int r,f;

    int tr = square / 8;
    int tf = square % 8;

    // maks relevent bishop occupancy bits
    for (r = tr + 1, f = tf + 1; r <= 6 && f <= 6; r++, f++)
        attacks |= (C64(1) << (r * 8 + f));
    for (r = tr - 1, f = tf + 1; r >= 1 && f <= 6; r--, f++)
        attacks |= (C64(1) << (r * 8 + f));
    for (r = tr + 1, f = tf - 1; r <= 6 && f >= 1; r++, f--)
        attacks |= (C64(1) << (r * 8 + f));
    for (r = tr - 1, f = tf - 1; r >= 1 && f >= 1; r--, f--)
        attacks |= (C64(1) << (r * 8 + f));

    return attacks;
}

U64 MaskRookAttacks(Square square) {
    U64 attacks = C64(0);

    int r,f;

    int tr = square / 8;
    int tf = square % 8;

    // maks relevent bishop occupancy bits
    for (r = tr + 1; r <= 6; r++)
        attacks |= (C64(1) << (r * 8 + tf));
    for (r = tr - 1; r >= 1; r--)
        attacks |= (C64(1) << (r * 8 + tf));
    for (f = tf + 1; f <= 6; f++)
        attacks |= (C64(1) << (tr * 8 + f));
    for (f = tf - 1; f >= 1; f--)
        attacks |= (C64(1) << (tr * 8 + f));

    return attacks;
}

U64 BishopAttacksOnTheFly(Square square, U64 block) {
    U64 attacks = C64(0);

    int r,f;

    int tr = square / 8;
    int tf = square % 8;

    // generate bishop attack
    for (r = tr + 1, f = tf + 1; r <= 7 && f <= 7; r++, f++) {
        attacks |= (C64(1) << (r * 8 + f));
        if ((C64(1) << (r * 8 + f)) & block) break;
    }
    for (r = tr - 1, f = tf + 1; r >= 0 && f <= 7; r--, f++) {
        attacks |= (C64(1) << (r * 8 + f));
        if ((C64(1) << (r * 8 + f)) & block) break;
    }
    for (r = tr + 1, f = tf - 1; r <= 7 && f >= 0; r++, f--) {
        attacks |= (C64(1) << (r * 8 + f));
        if ((C64(1) << (r * 8 + f)) & block) break;
    }
    for (r = tr - 1, f = tf - 1; r >= 0 && f >= 0; r--, f--) {
        attacks |= (C64(1) << (r * 8 + f));
        if ((C64(1) << (r * 8 + f)) & block) break;
    }

    return attacks;
}

U64 RookAttacksOnTheFly(Square square, U64 block) {
    U64 attacks = C64(0);

    int r,f;

    int tr = square / 8;
    int tf = square % 8;

    // maks relevent bishop ocycupancy bits
    for (r = tr + 1; r <= 7; r++) {
        attacks |= (C64(1) << (r * 8 + tf));
        if ((C64(1) << (r * 8 + tf)) & block) break;
    }
    for (r = tr - 1; r >= 0; r--) {
        attacks |= (C64(1) << (r * 8 + tf));
        if ((C64(1) << (r * 8 + tf)) & block) break;
    }
    for (f = tf + 1; f <= 7; f++) {
        attacks |= (C64(1) << (tr * 8 + f));
        if ((C64(1) << (tr * 8 + f)) & block) break;
    }
    for (f = tf - 1; f >= 0; f--) {
        attacks |= (C64(1) << (tr * 8 + f));
        if ((C64(1) << (tr * 8 + f)) & block) break;
    }

    return attacks;
}

// init leaper pieces attacks
void InitLeaperAttacks() {
    // loop over 64 board squares
    for (int square = Square::A1; square <= Square::H8; square++) {

        // init pawn attacks
        pawn_attacks[Side::White][square] = MaskPawnAttacks(Side::White, Square(square));
        pawn_attacks[Side::Black][square] = MaskPawnAttacks(Side::Black, Square(square));

        // init knight attacks
        knight_attacks[square] = MaskKnightAttacks(Square(square));

        // init king attacks
        king_attacks[square] = MaskKingAttacks(Square(square));

    }
}

/////////////////
// OCCUPANCIES //
/////////////////

U64 SetOccupancy(int index, int bits_in_mask, U64 attack_mask) {
    U64 occupancy = C64(0);

    for (int count = 0; count < bits_in_mask; count++) {
        int square = GetLsfbIndex(attack_mask);

        pop_bit(attack_mask, square);

        // make sure occupancey is on board
        if (index & (1 << count))
            // populate occupancy map
            occupancy |= (C64(1) << square);
    }

    return occupancy;

}

///////////////////
// MAGIC NUMBERS //
///////////////////

void InitSlidersAttacks(PieceType piece_type) {
    for (int square = Square::A1; square <= Square::H8; square++) {
        bishop_masks[square] = MaskBishopAttacks(Square(square));
        rook_masks[square] = MaskRookAttacks(Square(square));

        // init current mask
        U64 attack_mask = (piece_type == PieceType::Bishop) ? bishop_masks[square] : rook_masks[square];

        // init relevant occupancy bit count
        int relevant_bits_count = std::popcount(attack_mask);

        // init occupancy indices
        int occupancy_indices = (1 << relevant_bits_count);

        // loop over occupancy indices
        for (int index = 0; index < occupancy_indices; index++) {
            if (piece_type == PieceType::Bishop) {
                U64 occupancy = SetOccupancy(index, relevant_bits_count, attack_mask);
                int magic_index = occupancy * bishop_magics[square] >> (64 - bishop_relevant_bits[square]);
                bishop_attacks[square][magic_index] = BishopAttacksOnTheFly(Square(square), occupancy);
            } else if (piece_type == PieceType::Rook) {
                U64 occupancy = SetOccupancy(index, relevant_bits_count, attack_mask);
                int magic_index = occupancy * rook_magics[square] >> (64 - rook_relevant_bits[square]);
                rook_attacks[square][magic_index] = RookAttacksOnTheFly(Square(square), occupancy);
            }
        }

    }
}

static inline U64 GetBishopAttacks(Square square, U64 occupancy) {
    occupancy &= bishop_masks[square];
    occupancy *= bishop_magics[square];
    occupancy >>= 64 - bishop_relevant_bits[square];

    return bishop_attacks[square][occupancy];
}

static inline U64 GetRookAttacks(Square square, U64 occupancy) {
    occupancy &= rook_masks[square];
    occupancy *= rook_magics[square];
    occupancy >>= 64 - rook_relevant_bits[square];

    return rook_attacks[square][occupancy];
}

} // namespace gelato

//////////
// MAIN //
//////////


using namespace gelato;

int main() {
    InitLeaperAttacks();
    InitSlidersAttacks(PieceType::Rook);
    InitSlidersAttacks(PieceType::Bishop);

    Position position;

    // white pawns
    set_bit(position.bitboards[Piece::P], Square::A2);
    set_bit(position.bitboards[Piece::P], Square::B2);
    set_bit(position.bitboards[Piece::P], Square::C2);
    set_bit(position.bitboards[Piece::P], Square::D2);
    set_bit(position.bitboards[Piece::P], Square::E2);
    set_bit(position.bitboards[Piece::P], Square::F2);
    set_bit(position.bitboards[Piece::P], Square::G2);
    set_bit(position.bitboards[Piece::P], Square::H2);

    // white knights
    set_bit(position.bitboards[Piece::N], Square::B1);
    set_bit(position.bitboards[Piece::N], Square::G1);

    // white bishops
    set_bit(position.bitboards[Piece::B], Square::C1);
    set_bit(position.bitboards[Piece::B], Square::F1);

    // white rooks
    set_bit(position.bitboards[Piece::R], Square::A1);
    set_bit(position.bitboards[Piece::R], Square::H1);

    // king and queen
    set_bit(position.bitboards[Piece::K], Square::E1);
    set_bit(position.bitboards[Piece::Q], Square::D1);

    // white pawns
    set_bit(position.bitboards[Piece::p], Square::A7);
    set_bit(position.bitboards[Piece::p], Square::B7);
    set_bit(position.bitboards[Piece::p], Square::C7);
    set_bit(position.bitboards[Piece::p], Square::D7);
    set_bit(position.bitboards[Piece::p], Square::E7);
    set_bit(position.bitboards[Piece::p], Square::F7);
    set_bit(position.bitboards[Piece::p], Square::G7);
    set_bit(position.bitboards[Piece::p], Square::H7);

    // white knights
    set_bit(position.bitboards[Piece::n], Square::B8);
    set_bit(position.bitboards[Piece::n], Square::G8);

    // white bishops
    set_bit(position.bitboards[Piece::b], Square::C8);
    set_bit(position.bitboards[Piece::b], Square::F8);

    // white rooks
    set_bit(position.bitboards[Piece::r], Square::A8);
    set_bit(position.bitboards[Piece::r], Square::H8);

    // king and queen
    set_bit(position.bitboards[Piece::k], Square::E8);
    set_bit(position.bitboards[Piece::q], Square::D8);

    std::cout << position << std::endl;

    return 0;
}

