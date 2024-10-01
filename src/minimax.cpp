#include "engine.h"

int ChessEngine::minimax(int depth, int alpha, int beta, bool isMaximizing)
{
    if (depth == 0 || stopSearch.load())
    { // Check if search should stop
        return board.evaluateBoard();
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
