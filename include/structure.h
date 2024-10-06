#ifndef STRUCTURE_H
#define STRUCTURE_H

#include <iostream>
#include <sstream>
#include <vector>
#include <limits>
#include <string>
#include <atomic>
#include <array>
#include <stack>
#include <thread>
#include <unordered_map>
#include <random>
#include <cstdint>

const int INF = std::numeric_limits<int>::max();

enum PieceType
{
    EMPTY,
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING
};
enum Color
{
    NONE,
    WHITE,
    BLACK
};

struct Piece
{
    PieceType piece;
    Color color;

    Piece(PieceType p = EMPTY, Color c = NONE) : piece(p), color(c) {}

    bool operator==(const Piece &other) const
    {
        return piece == other.piece && color == other.color;
    }

    bool operator!=(const Piece &other) const
    {
        return piece != other.piece || color != other.color;
    }
};

struct Castle
{
    bool WKSC;
    bool WQSC;
    bool BKSC;
    bool BQSC;

    Castle(bool wksc, bool wqsc, bool bksc, bool bqsc) : WKSC(wksc), WQSC(wqsc), BKSC(bksc), BQSC(bqsc) {}
};

// Piece values for evaluation
const int pieceValues[7] = {0, 100, 320, 330, 500, 900, 1};

// color values form evaluation
const int colorValues[3] = {0, 1, -1};

const Color oppositeColor[3] = {NONE, BLACK, WHITE};

const std::string defaultFen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

enum Sqaure {
    A1, B1, C1, D1, E1, F1, G1, H1,
    A2, B2, C2, D2, E2, F2, G2, H2,
    A3, B3, C3, D3, E3, F3, G3, H3,
    A4, B4, C4, D4, E4, F4, G4, H4,
    A5, B5, C5, D5, E5, F5, G5, H5,
    A6, B6, C6, D6, E6, F6, G6, H6,
    A7, B7, C7, D7, E7, F7, G7, H7,
    A8, B8, C8, D8, E8, F8, G8, H8,
};

struct Move
{
    int from, to;
    Piece promotion;

    Move(int f, int t, Piece p = {EMPTY, NONE}) : from(f), to(t), promotion(p) {}

    std::string toString() const
    {
        // Calculate the file and rank for the from and to squares
        char fromFile = 'a' + (from % 8);
        char fromRank = '1' + (from / 8);
        char toFile = 'a' + (to % 8);
        char toRank = '1' + (to / 8);

        // Handle promotion pieces, if applicable
        char promotionChar = '\0';
        switch (promotion.piece)
        {
        case KNIGHT:
            promotionChar = 'n';
            break;
        case BISHOP:
            promotionChar = 'b';
            break;
        case ROOK:
            promotionChar = 'r';
            break;
        case QUEEN:
            promotionChar = 'q';
            break;
        default:
            promotionChar = '\0'; // No promotion
            break;
        }

        // Build the final move string
        std::string moveStr = std::string() + fromFile + fromRank + toFile + toRank;
        if (promotionChar != '\0') // Add promotion character if applicable
        {
            moveStr += promotionChar;
        }

        return moveStr;
    }
};

class ChessBoard
{
public:
    Piece board[64];
    Color colorToMove;
    std::stack<int> fiftyMoves;
    std::stack<int> enPassantTarget;
    std::stack<std::pair<Move, Piece>> moves;
    std::stack<Castle> castlingRights;
    std::unordered_map<uint64_t, int> positionCount;
    std::array<int, 3> kingPosition;

    ChessBoard(std::string fen = defaultFen)
    {
        std::istringstream iss(fen);
        std::vector<std::string> modified_fen;

        do
        {
            std::string subs;
            iss >> subs;
            modified_fen.push_back(subs);
        } while (iss);

        int file = 0, rank = 7;

        for (char s : modified_fen[0])
        {
            switch ((s))
            {
            case 'p':
                board[rank * 8 + file] = {PAWN, BLACK};
                file++;
                break;
            case 'n':
                board[rank * 8 + file] = {KNIGHT, BLACK};
                file++;
                break;
            case 'b':
                board[rank * 8 + file] = {BISHOP, BLACK};
                file++;
                break;
            case 'r':
                board[rank * 8 + file] = {ROOK, BLACK};
                file++;
                break;
            case 'q':
                board[rank * 8 + file] = {QUEEN, BLACK};
                file++;
                break;
            case 'k':
                board[rank * 8 + file] = {KING, BLACK};
                kingPosition[BLACK] = rank * 8 + file;
                file++;
                break;
            case 'P':
                board[rank * 8 + file] = {PAWN, WHITE};
                file++;
                break;
            case 'N':
                board[rank * 8 + file] = {KNIGHT, WHITE};
                file++;
                break;
            case 'B':
                board[rank * 8 + file] = {BISHOP, WHITE};
                file++;
                break;
            case 'R':
                board[rank * 8 + file] = {ROOK, WHITE};
                file++;
                break;
            case 'Q':
                board[rank * 8 + file] = {QUEEN, WHITE};
                file++;
                break;
            case 'K':
                board[rank * 8 + file] = {KING, WHITE};
                kingPosition[WHITE] = rank * 8 + file;
                file++;
                break;
            case '/':
                file = 0;
                rank--;
                break;
            default:
                int num = s - '0';
                for (int i = file; i < file + num; i++)
                {
                    board[rank * 8 + i] = {EMPTY, NONE};
                }
                file += num;
                break;
            }
        }

        if (modified_fen[1] == "w")
            colorToMove = WHITE;
        else
            colorToMove = BLACK;

        Castle castle = {false, false, false, false};
        for (char s : modified_fen[2])
        {
            switch (s)
            {
            case 'K':
                castle.WKSC = true;
                break;
            case 'Q':
                castle.WQSC = true;
                break;
            case 'k':
                castle.BKSC = true;
                break;
            case 'q':
                castle.BQSC = true;
                break;
            }
        }
        castlingRights.push(castle);

        if (modified_fen[3] == "-")
            enPassantTarget.push(-1);
        else
        {
            enPassantTarget.push((modified_fen[3][0] - 'a') + ((modified_fen[3][1] - '1') * 8));
        }

        fiftyMoves.push(stoi(modified_fen[4]));

        updatePositionCount();
    }

    void updatePositionCount();

    bool isThreefoldRepetition() const;

    void removePositionFromCount();

    void printBoard() const;

    bool checkAt(int index, Color forColor);

    std::vector<Move> generateLegalMoves();

    std::vector<Move> generateCapturePromotionMoves();

    std::vector<Move> generateCheckMoves();

    void makeMove(const Move &move);

    void unmakeMove();

    bool isPawnPromotion(const Move &move)
    {
        return (board[move.from].piece == PAWN && (move.to / 8 == 0 || move.to / 8 == 7));
    }

    double evaluateBoard() const;

    void makeMoveFromUCI(const std::string &moveStr);
};

#endif // STRUCTURE_H