#include "engine.h"

int ChessEngine::quiescence(int alpha, int beta, int depth)
{
    int bestValue = board.evaluateBoard() * colorValues[board.colorToMove];

    if (depth <= 0 || stopSearch.load())
        return bestValue;

    alpha = std::max(alpha, bestValue);

    if (alpha >= beta)
        return bestValue;

    std::vector<Move> captureMoves = board.generateTacticalMoves();

    for (const Move &move : captureMoves)
    {
        board.makeMove(move);
        int eval = -quiescence(-beta, -alpha, depth - 1);
        board.unmakeMove();

        bestValue = std::max(eval, bestValue);
        alpha = std::max(alpha, bestValue);

        if (alpha >= beta)
        {
            break;;
        }
    }

    return bestValue;
}

int ChessEngine::negamax(int depth, int alpha, int beta)
{
    if (depth == 0 || stopSearch.load())
    {
        return quiescence(alpha, beta, 0);
        // return board.evaluateBoard() * colorValues[board.colorToMove];
    }

    std::vector<Move> moves = board.generateLegalMoves();

    if (moves.empty())
    {
        if (board.checkAt(board.kingPosition[board.colorToMove], board.colorToMove))
        {
            // Checkmate condition: return a large negative value if in check
            return -INF;
        }
        else
        {
            // Stalemate condition: return 0
            return 0;
        }
    }

    int maxEval = -INF;

    for (const Move &move : moves)
    {
        board.makeMove(move);
        int eval = -negamax(depth - 1, -beta, -alpha);
        board.unmakeMove();

        maxEval = std::max(maxEval, eval);
        alpha = std::max(alpha, eval);
        if (alpha >= beta || stopSearch.load())
        {
            break;
        }
    }

    return maxEval;
}
