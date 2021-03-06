#pragma once

#include <string>
#include <regex>
#include <map>
#include "types.h"

namespace gelato {

namespace util {
	std::string GetCoordinatesFromSquare(Square square);
	bool ValidateFen(const std::string& fen);
	extern const std::map<char, Piece> CharToPiece;
	Square StringToSquare(std::string squareString);
} // namespace util

} // namespace gelato
