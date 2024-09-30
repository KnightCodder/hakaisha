#include "structure.h"

void ChessBoard::makeMoveFromUCI(const std::string &moveStr)
{
    // Translate UCI move string (e2e4, e7e8q) to internal move structure
    int from = (moveStr[0] - 'a') + ((moveStr[1] - '1') * 8);
    int to = (moveStr[2] - 'a') + ((moveStr[3] - '1') * 8);
    Piece promotion = {};

    if (moveStr.size() == 5)
    {
        char promChar = moveStr[4];
        switch (promChar)
        {
        case 'q':
            promotion = {QUEEN, colorToMove};
            break;
        case 'r':
            promotion = {ROOK, colorToMove};
            break;
        case 'b':
            promotion = {BISHOP, colorToMove};
            break;
        case 'n':
            promotion = {KNIGHT, colorToMove};
            break;
        }
    }

    Move move(from, to, promotion);
    makeMove(move);
}

void ChessBoard::printBoard() const
{
    for (int rank = 7; rank >= 0; --rank) // Ranks 8 to 1
    {
        std::cout << rank + 1 << " ";        // Print rank numbers
        for (int file = 0; file < 8; ++file) // Files a to h
        {
            Piece piece = board[rank * 8 + file]; // Calculate the position

            if (piece.color == Color::WHITE)
            {
                switch (piece.piece)
                {
                case PieceType::KING:
                    std::cout << 'K' << ' ';
                    break;
                case PieceType::QUEEN:
                    std::cout << 'Q' << ' ';
                    break;
                case PieceType::ROOK:
                    std::cout << 'R' << ' ';
                    break;
                case PieceType::BISHOP:
                    std::cout << 'B' << ' ';
                    break;
                case PieceType::KNIGHT:
                    std::cout << 'N' << ' ';
                    break;
                case PieceType::PAWN:
                    std::cout << 'P' << ' ';
                    break;
                case PieceType::EMPTY:
                    std::cout << '.' << ' ';
                    break;
                default:
                    std::cout << '?' << ' ';
                    break; // Unknown piece
                }
            }

            else if (piece.color == Color::BLACK)
            {
                switch (piece.piece)
                {
                case PieceType::KING:
                    std::cout << 'k' << ' ';
                    break;
                case PieceType::QUEEN:
                    std::cout << 'q' << ' ';
                    break;
                case PieceType::ROOK:
                    std::cout << 'r' << ' ';
                    break;
                case PieceType::BISHOP:
                    std::cout << 'b' << ' ';
                    break;
                case PieceType::KNIGHT:
                    std::cout << 'n' << ' ';
                    break;
                case PieceType::PAWN:
                    std::cout << 'p' << ' ';
                    break;
                case PieceType::EMPTY:
                    std::cout << '.' << ' ';
                    break;
                default:
                    std::cout << '?' << ' ';
                    break; // Unknown piece
                }
            }

            else
                std::cout << '.' << ' ';
        }
        std::cout << std::endl; // New line for next rank
    }
    std::cout << "  a b c d e f g h" << std::endl; // Print file letters

    std::cout << "\n\tenpassent : " << enPassantTarget.top() << std::endl;
    std::cout << "\tcastling (KQkq) : " << castlingRights.top().WKSC << castlingRights.top().WQSC << castlingRights.top().BKSC << castlingRights.top().BQSC << std::endl;
    std::cout << "\tthree fold repetition : " << isThreefoldRepetition() << std::endl;
    std::cout << "\tturn : " << colorToMove << std::endl;
    std::cout << "\tfifty moves : " << fiftyMoves.top() << '\n'
              << std::endl;
}
