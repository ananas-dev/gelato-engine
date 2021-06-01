#include "util.h"

namespace gelato
{

std::string GetCoordinatesFromSquare(Square s) {
    return std::string{ char('a' + GetFile(s)), char('1' + GetRank(s))};
};

bool ValidateFen(const std::string &fen) {
    return std::regex_match(fen, std::regex("\\s*([rnbqkpRNBQKP1-8]+\\/){7}([rnbqkpRNBQKP1-8]+)\\s[bw-]\\s(([a-hkqA-HKQ]{1,4})|(-))\\s(([a-h][36])|(-))\\s\\d+\\s\\d+\\s*"));
}

const std::map<char, Piece> CharToPiece = {
    {'P', Piece::P},
    {'N', Piece::N},
    {'B', Piece::B},
    {'R', Piece::R},
    {'Q', Piece::Q},
    {'K', Piece::K},
    {'p', Piece::p},
    {'n', Piece::n},
    {'b', Piece::b},
    {'r', Piece::r},
    {'q', Piece::q},
    {'k', Piece::k}
};

Square StringToSquare(std::string squareString) {
    int file = squareString[0] - 'a';
    int rank = (squareString[1] - '1');

    return GetSquare(Rank(rank), File(file));
}

} // namespace gelato