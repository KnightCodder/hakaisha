#include "structure.h"

void ChessBoard::makeMove(const Move &move)
{
    moves.push({move, board[move.to]});

    if (board[move.from].piece == KING)
        kingPosition[board[move.from].color] = move.to;

    // Handle pawn promotion
    if (isPawnPromotion(move))
    {
        board[move.to] = {move.promotion.piece, board[move.from].color}; // Replace pawn with the promotion piece
        board[move.from] = {};
        enPassantTarget.push(-1);
        fiftyMoves.push(0);
    }
    // Handle en passant
    else if (move.to == enPassantTarget.top() && board[move.from].piece == PieceType::PAWN)
    {
        int capturedPawnPosition = (board[move.from].color == WHITE) ? move.to - 8 : move.to + 8; // Adjust based on color
        board[capturedPawnPosition] = {};
        board[move.to] = board[move.from];
        board[move.from] = {};

        enPassantTarget.push(-1);
        fiftyMoves.push(0);
    }
    // Handle castling
    else if (board[move.from].piece == PieceType::KING && (move.from - move.to == 2 || move.to - move.from == 2))
    {
        // Move the king
        board[move.to] = board[move.from]; // Move king to its new position
        board[move.from] = {};             // Clear original king position

        // Move the rook based on castling direction
        int rookFrom = (move.to == move.from + 2) ? move.from + 3 : move.from - 4; // Determine rook's position
        int rookTo = (move.to == move.from + 2) ? move.to - 1 : move.to + 1;       // Determine rook's new position
        board[rookTo] = board[rookFrom];                                           // Move rook to its new position
        board[rookFrom] = {};                                                      // Clear original rook position

        enPassantTarget.push(-1);
        fiftyMoves.push(fiftyMoves.top() + 1);
    }
    // Standard move
    else
    {
        if (board[move.to].piece != PieceType::EMPTY)
        {
            fiftyMoves.push(0);
            enPassantTarget.push(0);
        }

        else if (board[move.from].piece == PieceType::PAWN)
        {
            fiftyMoves.push(0);

            if (move.to - move.from == 16 || move.from - move.to == 16)
            {
                if ((move.to % 8 > 0 && board[move.to - 1] == Piece{PieceType::PAWN, oppositeColor[board[move.from].color]}) || (move.to % 8 < 7 && board[move.to + 1] == Piece{PieceType::PAWN, oppositeColor[board[move.from].color]}))
                {
                    enPassantTarget.push(move.from + 8 * colorValues[board[move.from].color]);
                }
                else
                {
                    enPassantTarget.push(-1);
                }
            }
            else
                enPassantTarget.push(-1);
        }

        else
        {
            fiftyMoves.push(fiftyMoves.top() + 1);
            enPassantTarget.push(-1);
        }

        board[move.to] = board[move.from];
        board[move.from] = {};
    }

    Castle castle = castlingRights.top();
    if (move.from == 0 || move.to == 0)
        castle.WQSC = false;
    if (move.from == 7 || move.to == 7)
        castle.WKSC = false;
    if (move.from == 56 || move.to == 56)
        castle.BQSC = false;
    if (move.from == 63 || move.to == 63)
        castle.BKSC = false;
    if (move.from == 4)
        castle.WKSC = castle.WQSC = false;
    if (move.from == 60)
        castle.BKSC = castle.BQSC = false;
    castlingRights.push(castle);

    colorToMove = oppositeColor[colorToMove];

    updatePositionCount();
}

void ChessBoard::unmakeMove()
{
    removePositionFromCount();

    enPassantTarget.pop();
    fiftyMoves.pop();
    castlingRights.pop();

    Move move = moves.top().first;
    Piece capturedPiece = moves.top().second;

    moves.pop();

    if (board[move.to].piece == KING)
        kingPosition[board[move.to].color] = move.from;

    // Handle pawn promotion
    if (move.promotion != Piece{EMPTY, NONE}) // have to find a way to solve it
    {
        // printBoard();
        board[move.from] = {PAWN, board[move.to].color}; // Restore the pawn to its original position
        board[move.to] = capturedPiece;                  // Clear the promoted piece
        // printBoard();
    }
    // Handle en passant
    else if (move.to == enPassantTarget.top() && board[move.to].piece == PieceType::PAWN)
    {
        int capturedPawnPosition = (board[move.to].color == WHITE) ? move.to - 8 : move.to + 8; // Adjust based on color
        board[capturedPawnPosition] = {PAWN, oppositeColor[board[move.to].color]};
        board[move.from] = board[move.to];
        board[move.to] = {};
    }
    // Handle castling
    else if (board[move.to].piece == PieceType::KING && (move.from - move.to == 2 || move.to - move.from == 2))
    {
        // Move the king back
        board[move.from] = board[move.to]; // Move king back to original position
        board[move.to] = {};               // Clear the new king position

        // Move the rook back based on castling direction
        int rookFrom = (move.to == move.from + 2) ? move.to - 1 : move.to + 1;
        int rookTo = (move.to == move.from + 2) ? move.from + 3 : move.from - 4;
        board[rookTo] = board[rookFrom];
        board[rookFrom] = {};
    }
    // Standard unmake
    else
    {
        board[move.from] = board[move.to]; // Move the piece back to the original position
        board[move.to] = capturedPiece;    // Restore the captured piece
    }

    colorToMove = oppositeColor[colorToMove];
}
