#include "structure.h"

std::array<std::array<std::array<std::vector<int>, 8>, 64>, 7> precompiledPieceVision;

std::vector<Move> ChessBoard::generateLegalMoves()
{
    std::vector<Move> moves, tempMoves;

    for (int square = 0; square < 64; square++)
    {
        if (board[square].color != colorToMove)
            continue;

        if (board[square].piece == PAWN)
        {
            if (board[square].color == WHITE)
            {
                for (int tar : precompiledPieceVision[PAWN][square][0])
                {
                    if (board[tar].color != NONE)
                        break;

                    if (square > 47)
                    {
                        tempMoves.push_back({square, tar, QUEEN});
                        tempMoves.push_back({square, tar, ROOK});
                        tempMoves.push_back({square, tar, BISHOP});
                        tempMoves.push_back({square, tar, KNIGHT});
                    }
                    else
                        tempMoves.push_back({square, tar, EMPTY});
                }
                if (!precompiledPieceVision[PAWN][square][1].empty())
                {
                    if (board[precompiledPieceVision[PAWN][square][1].at(0)].color == oppositeColor[colorToMove] || precompiledPieceVision[PAWN][square][1].at(0) == enPassantTarget.top())
                    {
                        if (square > 47)
                        {
                            tempMoves.push_back({square, precompiledPieceVision[PAWN][square][1].at(0), QUEEN});
                            tempMoves.push_back({square, precompiledPieceVision[PAWN][square][1].at(0), ROOK});
                            tempMoves.push_back({square, precompiledPieceVision[PAWN][square][1].at(0), BISHOP});
                            tempMoves.push_back({square, precompiledPieceVision[PAWN][square][1].at(0), KNIGHT});
                        }
                        else
                            tempMoves.push_back({square, precompiledPieceVision[PAWN][square][1].at(0), EMPTY});
                    }
                }
                if (!precompiledPieceVision[PAWN][square][2].empty())
                {

                    if (board[precompiledPieceVision[PAWN][square][2].at(0)].color == oppositeColor[colorToMove] || precompiledPieceVision[PAWN][square][2].at(0) == enPassantTarget.top())
                    {
                        if (square > 47)
                        {

                            tempMoves.push_back({square, precompiledPieceVision[PAWN][square][2].at(0), QUEEN});
                            tempMoves.push_back({square, precompiledPieceVision[PAWN][square][2].at(0), ROOK});
                            tempMoves.push_back({square, precompiledPieceVision[PAWN][square][2].at(0), BISHOP});
                            tempMoves.push_back({square, precompiledPieceVision[PAWN][square][2].at(0), KNIGHT});
                        }
                        else
                            tempMoves.push_back({square, precompiledPieceVision[PAWN][square][2].at(0), EMPTY});
                    }
                }
            }
            else if (board[square].color == BLACK)
            {
                for (int tar : precompiledPieceVision[PAWN][square][3])
                {
                    if (board[tar].color != NONE)
                        break;

                    if (square < 16)
                    {
                        tempMoves.push_back({square, tar, QUEEN});
                        tempMoves.push_back({square, tar, ROOK});
                        tempMoves.push_back({square, tar, BISHOP});
                        tempMoves.push_back({square, tar, KNIGHT});
                    }
                    else
                        tempMoves.push_back({square, tar, EMPTY});
                }

                if (!precompiledPieceVision[PAWN][square][4].empty())
                    if (board[precompiledPieceVision[PAWN][square][4].at(0)].color == oppositeColor[colorToMove] || precompiledPieceVision[PAWN][square][4].at(0) == enPassantTarget.top())
                    {
                        if (square < 16)
                        {
                            tempMoves.push_back({square, precompiledPieceVision[PAWN][square][4].at(0), QUEEN});
                            tempMoves.push_back({square, precompiledPieceVision[PAWN][square][4].at(0), ROOK});
                            tempMoves.push_back({square, precompiledPieceVision[PAWN][square][4].at(0), BISHOP});
                            moves.push_back({square, precompiledPieceVision[PAWN][square][4].at(0), KNIGHT});
                        }
                        else
                            tempMoves.push_back({square, precompiledPieceVision[PAWN][square][4].at(0), EMPTY});
                    }

                if (!precompiledPieceVision[PAWN][square][5].empty())
                    if (board[precompiledPieceVision[PAWN][square][5].at(0)].color == oppositeColor[colorToMove] || precompiledPieceVision[PAWN][square][5].at(0) == enPassantTarget.top())
                    {
                        if (square < 16)
                        {

                            tempMoves.push_back({square, precompiledPieceVision[PAWN][square][5].at(0), QUEEN});
                            tempMoves.push_back({square, precompiledPieceVision[PAWN][square][5].at(0), ROOK});
                            tempMoves.push_back({square, precompiledPieceVision[PAWN][square][5].at(0), BISHOP});
                            tempMoves.push_back({square, precompiledPieceVision[PAWN][square][5].at(0), KNIGHT});
                        }
                        else
                            tempMoves.push_back({square, precompiledPieceVision[PAWN][square][5].at(0), EMPTY});
                    }
            }
        }

        else
        {
            for (int dir = 0; dir < 8; dir++)
            {
                for (int tar : precompiledPieceVision[board[square].piece][square][dir])
                {
                    if (board[tar].color == colorToMove)
                        break;

                    tempMoves.push_back({square, tar});

                    if (board[tar].color != NONE)
                        break;
                }
            }
        }
    }

    // remove illegal moves
    for (Move move : tempMoves)
    {
        makeMove(move);

        if (!checkAt(kingPosition[oppositeColor[colorToMove]], oppositeColor[colorToMove]))
        {
            moves.push_back(move);
        }

        unmakeMove();
    }

    {
        int check = -1;
        // handle castle
        if (colorToMove == WHITE)
        {
            if (castlingRights.top().WKSC && board[5].piece == EMPTY && board[6].piece == EMPTY)
            {
                check = checkAt(4, WHITE);

                if (!check)
                {
                    if (!checkAt(5, WHITE) && !checkAt(6, WHITE))
                        moves.push_back({4, 6});
                }
            }
            if (castlingRights.top().WQSC && board[3].piece == EMPTY && board[2].piece == EMPTY && board[1].piece == EMPTY)
            {
                if (check == -1)
                    check = checkAt(4, WHITE);
                if (!check)
                {
                    if (!checkAt(3, WHITE) && !checkAt(2, WHITE))
                        moves.push_back({4, 2});
                }
            }
        }

        else if (colorToMove == BLACK)
        {
            if (castlingRights.top().BKSC && board[61].piece == EMPTY && board[62].piece == EMPTY)
            {
                check = checkAt(60, BLACK);

                if (!check)
                {
                    if (!checkAt(61, BLACK) && !checkAt(62, BLACK))
                        moves.push_back({60, 62});
                }
            }
            if (castlingRights.top().BQSC && board[59].piece == EMPTY && board[58].piece == EMPTY && board[57].piece == EMPTY)
            {
                if (check == -1)
                    check = checkAt(60, BLACK);
                if (!check)
                {
                    if (!checkAt(59, BLACK) && !checkAt(58, BLACK))
                        moves.push_back({60, 58});
                }
            }
        }
    }

    return moves;
}

std::vector<Move> ChessBoard::generateTacticalMoves()
{
    std::vector<Move> tacticalMoves, tempMoves;
    // int opponentKingSquare = kingPosition[oppositeColor[colorToMove]];

    for (int square = 0; square < 64; square++)
    {
        if (board[square].color != colorToMove)
            continue;

        // Process all potential moves for the piece at the square
        for (int dir = 0; dir < 8; dir++)
        {
            for (int target : precompiledPieceVision[board[square].piece][square][dir])
            {
                if (board[target].color == colorToMove)
                    break; // Stop if the target square has our own piece

                // Captures (target square is occupied by opponent's piece)
                if (board[target].color == oppositeColor[colorToMove])
                {
                    tempMoves.push_back({square, target, EMPTY}); // Capture move
                    break;                                            // Stop further exploration in this direction after capture
                }

                // Promotions (if a pawn reaches the back rank)
                // if (board[square].piece == PAWN)
                // {
                //     // White pawn reaches rank 8 or Black pawn reaches rank 1
                //     if ((board[square].color == WHITE && target > 47) || (board[square].color == BLACK && target < 16))
                //     {
                //         tacticalMoves.push_back({square, target, QUEEN});
                //         tacticalMoves.push_back({square, target, ROOK});
                //         tacticalMoves.push_back({square, target, BISHOP});
                //         tacticalMoves.push_back({square, target, KNIGHT});
                //         break; // Stop further exploration after promotion
                //     }
                // }

                // // Check if the move puts the opponent's king in check
                // makeMove({square, target});
                // if (checkAt(opponentKingSquare, colorToMove))
                // {
                //     tacticalMoves.push_back({square, target, EMPTY}); // Add the check move
                // }
                // unmakeMove();

                // If the target square is occupied, stop searching further
                if (board[target].color != NONE)
                    break;
            }
        }
    }

    // remove illegal moves
    for (Move move : tempMoves)
    {
        makeMove(move);

        if (!checkAt(kingPosition[oppositeColor[colorToMove]], oppositeColor[colorToMove]))
        {
            tacticalMoves.push_back(move);
        }

        unmakeMove();
    }

    return tacticalMoves;
}

bool ChessBoard::checkAt(int index, Color forColor)
{
    for (int piece = KNIGHT; piece <= KING; piece++)
    {
        for (int dir = 0; dir < 8; dir++)
        {
            for (int tar : precompiledPieceVision[piece][index][dir])
            {
                if (board[tar].color == forColor)
                    break;

                if (board[tar].piece == piece && board[tar].color == oppositeColor[forColor])
                    return true;

                if (board[tar].color != NONE)
                    break;
            }
        }
    }

    std::array<std::vector<int>, 8> kingVision = precompiledPieceVision[KING][index];
    // for pawn
    if (forColor == WHITE)
    {
        if (!kingVision[5].empty() && board[kingVision[5].at(0)] == Piece{PAWN, BLACK})
            return true;
        if (!kingVision[4].empty() && board[kingVision[4].at(0)] == Piece{PAWN, BLACK})
            return true;
    }
    else if (forColor == BLACK)
    {
        if (!kingVision[6].empty() && board[kingVision[6].at(0)] == Piece{PAWN, WHITE})
            return true;
        if (!kingVision[7].empty() && board[kingVision[7].at(0)] == Piece{PAWN, WHITE})
            return true;
    }

    return false;
}
