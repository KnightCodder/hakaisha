#include "structure.h"

class ChessEngine
{
public:
    ChessBoard board;
    std::atomic<bool> stopSearch; // Atomic flag to stop search
    std::thread searchThread;

    ChessEngine() : stopSearch(false) {}

    int minimax(int depth, int alpha, int beta, bool isMaximizing);

    Move findBestMove(int depth)
    {
        std::vector<Move> moves = board.generateLegalMoves();
        Move bestMove = moves[0];
        int bestValue = -INF;

        for (const Move &move : moves)
        {
            board.makeMove(move);

            int moveValue = minimax(depth - 1, -INF, INF, false);
            board.unmakeMove();

            if (moveValue > bestValue)
            {
                bestValue = moveValue;
                bestMove = move;
            }

            if (stopSearch.load())
            { // Stop search if requested
                break;
            }
        }

        return bestMove;
    }

    void startSearch(int depth)
    {
        stop();
        stopSearch.store(false);
        searchThread = std::thread([this, depth]()
                                   {
            Move bestMove = findBestMove(depth);
            if (!stopSearch.load()) {  // Only output bestmove if search wasn't stopped
                std::cout << "bestmove " << bestMove.toString() << std::endl;
            } });
    }

    void stop()
    {
        stopSearch.store(true);
        if (searchThread.joinable())
        {
            searchThread.join(); // Ensure the search thread completes
        }
    }
};