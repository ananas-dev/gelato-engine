#include "position.h"

namespace gelato {

Position::Position() {
    //todo
}

Position::Position(std::string fen) {
    this->fen = fen;
}

std::ostream& operator<<(std::ostream &stream, const Position position) {
    std::string ascii_pieces = "PNBRQKpnbrqk";
    int piece;
    int square;
    stream << "+---+---+---+---+---+---+---+---+\n";
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            square = rank * 8 + file;
            piece = -1;
            for (int bb_piece = Piece::P; bb_piece <= Piece::k; bb_piece++) {
                if (get_bit(position.bitboards[bb_piece], square))
                    piece = bb_piece;
            }

            if (piece != -1)
                stream << "| " << ascii_pieces[piece] << " ";
            else
                stream << "|   ";
        }

        stream << "| " << (8 - rank) << "\n+---+---+---+---+---+---+---+---+\n";
;
    }

    stream << "  a   b   c   d   e   f   g   h\n\n";

    stream << "Turn: " << ((position.side == Side::White) ? "white" : "black") << std::endl;
    stream << "Enpassant: " << ((position.enpassant != Square::None)
        ? util::GetCoordinatesFromSquare(position.enpassant)
        :"None"
    ) << std::endl;
    stream << "Castle: "
        << ((position.castle & Castle::Wks) ? 'K' : '-')
        << ((position.castle & Castle::Wqs) ? 'Q' : '-')
        << ((position.castle & Castle::Bks) ? 'k' : '-')
        << ((position.castle & Castle::Bqs) ? 'q' : '-')
        << std::endl;

    return stream;
}

} // namespace gelato