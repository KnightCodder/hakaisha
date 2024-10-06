#include "engine.h"

double ChessEngine::quiescenceCaptures(double alpha, double beta, int depth)
{
    double bestValue = board.evaluateBoard() * colorValues[board.colorToMove];

    if (depth <= 0 || stopSearch.load())
        return bestValue;

    alpha = std::max(alpha, bestValue);

    if (alpha >= beta)
        return bestValue;

    std::vector<Move> captureMoves = board.generateCapturePromotionMoves();

    for (const Move &move : captureMoves)
    {
        board.makeMove(move);
        double eval = -quiescenceCaptures(-beta, -alpha, depth - 1);
        board.unmakeMove();

        bestValue = std::max(eval, bestValue);
        alpha = std::max(alpha, bestValue);

        if (alpha >= beta)
        {
            break;
        }
    }

    return bestValue;
}

double ChessEngine::quiescenceChecks(double alpha, double beta, int depth)
{
    double bestValue = board.evaluateBoard() * colorValues[board.colorToMove];

    if (depth <= 0 || stopSearch.load())
        return bestValue;

    alpha = std::max(alpha, bestValue);

    if (alpha >= beta)
        return bestValue;

    std::vector<Move> checkMoves = board.generateCheckMoves();

    for (const Move &move : checkMoves)
    {
        board.makeMove(move);
        double eval = -quiescenceChecks(-beta, -alpha, depth - 1);
        board.unmakeMove();

        bestValue = std::max(eval, bestValue);
        alpha = std::max(alpha, bestValue);

        if (alpha >= beta)
        {
            break;
        }
    }

    return bestValue;
}

double ChessEngine::negamax(int depth, double alpha, double beta)
{
    if (depth == 0 || stopSearch.load())
    {
        // return std::max(quiescenceChecks(alpha, beta, 1), quiescenceCaptures(alpha, beta, 1));
        return quiescenceCaptures(alpha, beta, 1);
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

    double maxEval = -INF;

    for (const Move &move : moves)
    {
        board.makeMove(move);
        double eval = -negamax(depth - 1, -beta, -alpha);
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
