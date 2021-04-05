#include "util.h"

namespace gelato {

namespace util {

std::string GetCoordinatesFromSquare(Square s) {
    return std::string{ char('a' + GetFile(s)), char('1' + GetRank(s))};
};

}

}