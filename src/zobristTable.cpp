#include "structure.h"

// Zobrist table for hashing board positions
uint64_t zobristTable[64][7][3]; // 64 squares, 7 piece types, 3 colors (including NONE)
uint64_t zobristBlackToMove;
uint64_t zobristCastlingRights[4]; // WK, WQ, BK, BQ
uint64_t zobristEnPassant[8];      // Files a-h for en passant

// Initialize Zobrist table with random numbers
void initializeZobrist()
{
    std::mt19937_64 generator(12345); // Fixed seed for reproducibility
    std::uniform_int_distribution<uint64_t> distribution(0, UINT64_MAX);

    for (int square = 0; square < 64; ++square)
    {
        for (int pieceType = 0; pieceType < 7; ++pieceType)
        {
            for (int color = 0; color < 3; ++color)
            {
                zobristTable[square][pieceType][color] = distribution(generator);
            }
        }
    }

    zobristBlackToMove = distribution(generator);
    for (int i = 0; i < 4; ++i)
    {
        zobristCastlingRights[i] = distribution(generator);
    }
    for (int i = 0; i < 8; ++i)
    {
        zobristEnPassant[i] = distribution(generator);
    }
}

uint64_t computeZobristHash(const ChessBoard &board)
{
    uint64_t hash = 0;

    // Add piece positions
    for (int square = 0; square < 64; ++square)
    {
        Piece piece = board.board[square];
        if (piece.piece != EMPTY)
        {
            hash ^= zobristTable[square][piece.piece][piece.color];
        }
    }

    // Add the side to move
    if (board.colorToMove == BLACK)
    {
        hash ^= zobristBlackToMove;
    }

    // Add castling rights
    const Castle &castling = board.castlingRights.top();
    if (castling.WKSC)
        hash ^= zobristCastlingRights[0];
    if (castling.WQSC)
        hash ^= zobristCastlingRights[1];
    if (castling.BKSC)
        hash ^= zobristCastlingRights[2];
    if (castling.BQSC)
        hash ^= zobristCastlingRights[3];

    // Add en passant file
    if (!board.enPassantTarget.empty())
    {
        int enPassantSquare = board.enPassantTarget.top();
        if (enPassantSquare >= 0 && enPassantSquare < 64)
        {
            int file = enPassantSquare % 8;
            hash ^= zobristEnPassant[file];
        }
    }

    return hash;
}

void ChessBoard::updatePositionCount()
{
    uint64_t hash = computeZobristHash(*this);
    positionCount[hash]++;
}

bool ChessBoard::isThreefoldRepetition() const
{
    uint64_t hash = computeZobristHash(*this);
    return positionCount.at(hash) >= 3;
}

void ChessBoard::removePositionFromCount()
{
    uint64_t hash = computeZobristHash(*this);
    if (positionCount[hash] > 0)
    {
        positionCount[hash]--;
    }
}
