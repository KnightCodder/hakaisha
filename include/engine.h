#include "structure.h"
#include <random>
#include <algorithm> // For std::shuffle

class ChessEngine
{
public:
    ChessBoard board;
    std::atomic<bool> stopSearch; // Atomic flag to stop search
    std::thread searchThread;

    ChessEngine() : stopSearch(false) {}

    int negamax(int depth, int alpha, int beta, int color);

    int quiescence(int alpha, int beta, bool isMaximizing);

    int minimax(int depth, int alpha, int beta, bool isMaximizing);

    Move findBestMove(int depth)
    {
        std::vector<Move> moves = board.generateLegalMoves();

        // Random number generator
        std::random_device rd;
        std::mt19937 gen(rd());

        // Shuffle the vector
        std::shuffle(moves.begin(), moves.end(), gen);

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

            // alpha = std::max(alpha, moveValue); // Update alpha for pruning

            // if (alpha >= beta || stopSearch.load())
            if (stopSearch.load())
            {
                // Beta cut-off or stop search if requested
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