#include "structure.h"

int ChessBoard::evaluateBoard() const
{
    if (isThreefoldRepetition())
        return 0;
    if (fiftyMoves.top() > 100)
        return 0;

    int score = 0;
    for (int i = 0; i < 64; ++i)
    {
        score += pieceValues[board[i].piece] * colorValues[board[i].color];
    }
    return colorToMove == WHITE ? score : -score;
}
