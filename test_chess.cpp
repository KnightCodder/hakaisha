#include <iostream>
#include "UCI_protocol.h"

void initializeZobrist();
void compilingPieceVision();

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

    ChessEngine haki;

    haki.board = ChessBoard("4rbr1/pp1nN3/1qp2n1k/6N1/3P4/3Q4/PPP2PPP/R5K1 w - - 0 22");

    Move bestmove = haki.findBestMove(4);

    printMove(bestmove);

    return 0;
}
