#include "position.h"

namespace gelato {

Position::Position() {
    this->Clear();
}

Position::Position(std::string fen) {
    this->LoadFen(fen);
}

void Position::Clear() {
    this->bitboards.fill(C64(0));
    this->occupancies.fill(C64(0));
    
    this->side = Side::White;
    this->enpassant = Square::None;

    // no castle by default
    this->castle = (Castle)0b0000;

    this->fen = "";
}

void Position::LoadFen(std::string fen) {
    this->Clear();
    if (ValidateFen(fen)) {
        // TODO: fen sanitizer
        this->fen = fen;

        std::regex whiteSpace("\\s+");

        // a number between 1 and 8
        std::string oneToEight = "12345678";
        std::string asciiPiece = "PNBRQKpnbrqk";

        std::sregex_token_iterator begin(fen.begin(), fen.end(), whiteSpace, -1);
        std::sregex_token_iterator end;

        std::vector<std::string> tokens(begin, end);

        int square = Square::H8;

        for (const char &piecePlacement: tokens[Fen::PiecePlacement]) {
            if (asciiPiece.find(piecePlacement) != std::string::npos) {
                bitboard::SetBit(this->bitboards[CharToPiece.at(piecePlacement)], (Square)square);
                square--;
            } else if (oneToEight.find(piecePlacement) != std::string::npos) {
                square -= piecePlacement - '0';
            }
        }

        switch (tokens[Fen::SideToMove][0]) {
            case 'w':
                this->side = Side::White;
                break;
            case 'b':
                this->side = Side::Black;
                break;
            default:
                this->side = Side::Both;
                break;
        }

        for (const char &castlingAbility: tokens[Fen::CastlingAbility]) {
            switch (castlingAbility) {
                case 'K':
                    this->castle = Castle(this->castle | Castle::Wks);
                    break;
                case 'Q':
                    this->castle = Castle(this->castle | Castle::Wqs);
                    break;
                case 'k':
                    this->castle = Castle(this->castle | Castle::Bks);
                    break;
                case 'q':
                    this->castle = Castle(this->castle | Castle::Bqs);
                    break;
                case '-':
                    break;
            }
        }

        if (tokens[Fen::EnPassantTargetSquare] != "-") {
            this->enpassant = StringToSquare(tokens[Fen::EnPassantTargetSquare]);
        }

        // init occupancy for white
        for (int piece = Piece::P; piece <= Piece::K; piece++) {
            this->occupancies[Side::White] |= bitboards[piece];
        }

        // init occupancy for black
        for (int piece = Piece::p; piece <= Piece::k; piece++) {
            this->occupancies[Side::Black] |= bitboards[piece];
        }

        // init all occupancies
        this->occupancies[Side::Both] |= occupancies[Side::White];
        this->occupancies[Side::Both] |= occupancies[Side::Black];
    } else {
        // TODO: handle error
    };
}

std::ostream& operator<<(std::ostream &stream, const Position position) {
    std::string ascii_pieces = "PNBRQKpnbrqk";
    int piece;
    Square square;
    stream << "+---+---+---+---+---+---+---+---+\n";
    for (int rank = Rank::Rank8; rank >= Rank::Rank1; rank--) {
        for (int file = File::File1; file <= File::File8; file++) {
            square = GetSquare(Rank(rank), File(file));
            piece = -1;
            for (int bb_piece = Piece::P; bb_piece <= Piece::k; bb_piece++) {
                if (bitboard::GetBit(position.bitboards[bb_piece], square))
                    piece = bb_piece;
            }

            if (piece != -1)
                stream << "| " << ascii_pieces[piece] << " ";
            else
                stream << "|   ";
        }

        stream << "| " << (rank + 1) << "\n+---+---+---+---+---+---+---+---+\n";
    }

    stream << "  a   b   c   d   e   f   g   h\n\n";

    stream << "Turn: " << ((position.side == Side::White) ? "white" : "black") << std::endl;
    stream << "Enpassant: " << ((position.enpassant != Square::None)
        ? GetCoordinatesFromSquare(position.enpassant)
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