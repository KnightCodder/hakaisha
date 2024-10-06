#include "structure.h"

std::array<double, 67> coefficients;
double intercept;

double ChessBoard::evaluateBoard() const
{
    if (isThreefoldRepetition())
        return 0;
    if (fiftyMoves.top() > 100)
        return 0;

    int piece_count = 0;
    double score = 0;
    int material = 0;
    for (int i = 0; i < 64; ++i)
    {
        material += pieceValues[board[i].piece] * colorValues[board[i].color];
        score += pieceValues[board[i].piece] * colorValues[board[i].color] * coefficients[i];
        if (board[i].color != NONE)
            piece_count++;
    }
    score += colorValues[colorToMove] * coefficients[64];
    score += piece_count * coefficients[65];
    score += material * coefficients[66];
    score += intercept;

    return score;
}
