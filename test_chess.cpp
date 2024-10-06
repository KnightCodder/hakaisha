#include <iostream>
#include "UCI_protocol.h"

void initializeZobrist();
void compilingPieceVision();
void initializeMLmodel();

void printMove(const Move& move) {
    char fromFile = 'a' + (move.from % 8);
    char fromRank = '1' + (move.from / 8);
    char toFile = 'a' + (move.to % 8);
    char toRank = '1' + (move.to / 8);

    std::cout << fromFile << fromRank << " - " << toFile << toRank << std::endl;
}

void testChessBoard() {
    ChessBoard chessBoard;

    while (true) {
        chessBoard.printBoard();

        std::vector<Move> moves = chessBoard.generateLegalMoves();
        
        std::cout << "Legal moves: " << std::endl;
        for (const Move& move : moves) {
            printMove(move);
        }

        std::string moveStr;
        std::cout << "Make a move (e.g., e2e4) or type 'undo' (or 'exit' to quit): ";
        std::cin >> moveStr;

        if (moveStr == "exit") {
            break; // Exit the loop if the user types 'exit'
        } else if (moveStr == "undo") {
            chessBoard.unmakeMove();
        } else {
            chessBoard.makeMoveFromUCI(moveStr);
        }
    }
}

int main() {
    initializeZobrist(); // Ensure Zobrist hashing is initialized
    compilingPieceVision();
    initializeMLmodel();

    ChessEngine haki;

    haki.board = ChessBoard("2r2k2/1P6/4P3/8/8/2p1B3/7p/3K4 b - - 0 1");

    std::vector<Move> moves = haki.board.generateCheckMoves();

    Move bestmove = haki.findBestMove(4);

    haki.board.printBoard();

    for(Move move : moves)
    {
        std::cout << "\t" << move.toString() << std::endl;
    }

    printMove(bestmove);

    return 0;
}
