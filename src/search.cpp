#include "engine.h"

int ChessEngine::quiescence(int alpha, int beta, bool isMaximizing)
{
    // return board.evaluateBoard(isMaximizing);

    std::vector<Move> captureMoves = board.generateTacticalMoves();

    if (captureMoves.empty() || stopSearch.load())
        return board.evaluateBoard(isMaximizing);

    if (isMaximizing)
    {
        int maxEval = -INF;

        for (const Move &move : captureMoves)
        {
            board.makeMove(move);
            int eval = quiescence(alpha, beta, false);
            board.unmakeMove();

            maxEval = std::max(maxEval, eval);
            alpha = std::max(alpha, eval);
            if (beta <= alpha || stopSearch.load())
            {
                break;
            }
        }

        return maxEval;
    }
    else
    {
        int minEval = INF;

        for (const Move &move : captureMoves)
        {
            board.makeMove(move);

            int eval = quiescence(alpha, beta, true);
            board.unmakeMove();

            minEval = std::min(minEval, eval);
            beta = std::min(beta, eval);
            if (beta <= alpha || stopSearch.load())
            {
                break;
            }
        }
        return minEval;
    }
}

int ChessEngine::minimax(int depth, int alpha, int beta, bool isMaximizing)
{
    if (depth == 0 || stopSearch.load())
    { // Check if search should stop
        return quiescence(alpha, beta, isMaximizing);
        // return board.evaluateBoard(isMaximizing);
    }

    std::vector<Move> moves = board.generateLegalMoves();

    if (isMaximizing)
    {
        int maxEval = -INF;

        if (moves.empty())
        {
            if (board.checkAt(board.kingPosition[board.colorToMove], board.colorToMove))
            {
                return maxEval;
            }
            else
                return 0;
        }

        for (const Move &move : moves)
        {
            board.makeMove(move);

            int eval = minimax(depth - 1, alpha, beta, false);
            board.unmakeMove();

            maxEval = std::max(maxEval, eval);
            alpha = std::max(alpha, eval);
            if (beta <= alpha || stopSearch.load())
            {
                break;
            }
        }
        return maxEval;
    }
    else
    {
        int minEval = INF;

        if (moves.empty())
        {
            if (board.checkAt(board.kingPosition[board.colorToMove], board.colorToMove))
            {
                return minEval;
            }
            else
                return 0;
        }

        for (const Move &move : moves)
        {
            board.makeMove(move);

            int eval = minimax(depth - 1, alpha, beta, true);
            board.unmakeMove();

            minEval = std::min(minEval, eval);
            beta = std::min(beta, eval);
            if (beta <= alpha || stopSearch.load())
            {
                break;
            }
        }
        return minEval;
    }
}

int ChessEngine::negamax(int depth, int alpha, int beta, int color)
{
    if (depth == 0 || stopSearch.load())
    {
        return quiescence(alpha, beta, color);
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
        int eval = -negamax(depth - 1, -beta, -alpha, -color);
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
