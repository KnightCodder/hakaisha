#include "structure.h"

extern std::array<std::array<std::array<std::vector<int>, 8>, 64>, 7> precompiledPieceVision;

const int knightOffsets[8] = {15, 17, 10, 6, -15, -17, -10, -6};
const int kingOffsets[8] = {1, -1, 8, -8, 9, 7, -9, -7};
const int rookOffsets[4] = {1, -1, 8, -8};
const int bishopOffsets[4] = {9, 7, -9, -7};
const int queenOffsets[8] = {1, -1, 8, -8, 9, 7, -9, -7};

// Pawn offsets
const int whitePawnMove = 8;          // Move one square forward
const int whitePawnCaptureLeft = 7;   // Capture left diagonal
const int whitePawnCaptureRight = 9;  // Capture right diagonal
const int blackPawnMove = -8;         // Move one square forward
const int blackPawnCaptureLeft = -9;  // Capture left diagonal
const int blackPawnCaptureRight = -7; // Capture right diagonal

// Function to check if a square is valid (on the board)
bool isValidSquare(int square)
{
    return square >= 0 && square < 64;
}

void compilingPieceVision()
{
    for (int piece = PAWN; piece <= KING; ++piece)
    {
        for (int square = 0; square < 64; ++square)
        {
            if (piece == KNIGHT)
            {
                // Precompute knight moves
                for (int dir = 0; dir < 8; ++dir)
                {
                    int targetSquare = square + knightOffsets[dir];
                    if (isValidSquare(targetSquare) && abs((square % 8) - (targetSquare % 8)) <= 2)
                    {
                        precompiledPieceVision[KNIGHT][square][dir].push_back(targetSquare);
                    }
                }
            }
            else if (piece == KING)
            {
                // Precompute king moves (1 square in any direction)
                for (int dir = 0; dir < 8; ++dir)
                {
                    int targetSquare = square + kingOffsets[dir];
                    if (!isValidSquare(targetSquare))
                        continue;
                    if (dir == 0 && targetSquare % 8 == 0)
                        continue;
                    if (dir == 1 && (targetSquare + 1) % 8 == 0)
                        continue;
                    if ((dir == 4 || dir == 7) && targetSquare % 8 == 0)
                        continue;
                    if ((dir == 5 || dir == 6) && (targetSquare + 1) % 8 == 0)
                        continue;
                    {
                        precompiledPieceVision[KING][square][dir].push_back(targetSquare);
                    }
                }
            }
            else if (piece == ROOK)
            {
                // Precompute rook moves (along ranks and files)
                for (int dir = 0; dir < 4; ++dir)
                {
                    int targetSquare = square;
                    while (true)
                    {
                        targetSquare += rookOffsets[dir];
                        if (!isValidSquare(targetSquare))
                            break;
                        if (dir == 0 && targetSquare % 8 == 0)
                            break;
                        if (dir == 1 && (targetSquare + 1) % 8 == 0)
                            break;

                        precompiledPieceVision[ROOK][square][dir].push_back(targetSquare);
                    }
                }
            }
            else if (piece == BISHOP)
            {
                // Precompute bishop moves (along diagonals)
                for (int dir = 0; dir < 4; ++dir)
                {
                    int targetSquare = square;
                    while (true)
                    {
                        targetSquare += bishopOffsets[dir];
                        if (!isValidSquare(targetSquare))
                            break;
                        if ((dir == 0 || dir == 3) && targetSquare % 8 == 0)
                            break;
                        if ((dir == 1 || dir == 2) && (targetSquare + 1) % 8 == 0)
                            break;
                        precompiledPieceVision[BISHOP][square][dir].push_back(targetSquare);
                    }
                }
            }
            else if (piece == QUEEN)
            {
                // Precompute queen moves (combines rook and bishop)
                for (int dir = 0; dir < 8; ++dir)
                {
                    int targetSquare = square;
                    while (true)
                    {
                        targetSquare += queenOffsets[dir];
                        if (!isValidSquare(targetSquare))
                            break;
                        if (dir == 0 && targetSquare % 8 == 0)
                            break;
                        if (dir == 1 && (targetSquare + 1) % 8 == 0)
                            break;
                        if ((dir == 4 || dir == 7) && targetSquare % 8 == 0)
                            break;
                        if ((dir == 5 || dir == 6) && (targetSquare + 1) % 8 == 0)
                            break;
                        precompiledPieceVision[QUEEN][square][dir].push_back(targetSquare);
                    }
                }
            }
            else if (piece == PAWN)
            {
                // Precompute white pawn moves and captures
                if (square >= 8 && square < 56) // Only consider non-first and non-last ranks
                {
                    // White pawn move
                    int targetSquare = square + whitePawnMove;
                    if (isValidSquare(targetSquare))
                        precompiledPieceVision[PAWN][square][0].push_back(targetSquare);
                    if (square > 7 && square < 16)
                        precompiledPieceVision[PAWN][square][0].push_back(square + 16);

                    // White pawn captures
                    int leftCapture = square + whitePawnCaptureLeft;
                    int rightCapture = square + whitePawnCaptureRight;
                    if (isValidSquare(leftCapture) && (square % 8 != 0)) // Not on right edge
                        precompiledPieceVision[PAWN][square][1].push_back(leftCapture);
                    if (isValidSquare(rightCapture) && (square % 8 != 7)) // Not on left edge
                        precompiledPieceVision[PAWN][square][2].push_back(rightCapture);
                }

                // Precompute black pawn moves and captures
                if (square >= 8 && square < 56) // Only consider non-first and non-last ranks
                {
                    // Black pawn move
                    int targetSquare = square + blackPawnMove;
                    if (isValidSquare(targetSquare))
                        precompiledPieceVision[PAWN][square][3].push_back(targetSquare);
                    if (square > 47 && square < 56)
                        precompiledPieceVision[PAWN][square][3].push_back(square - 16);
                    // Black pawn captures
                    int leftCapture = square + blackPawnCaptureLeft;
                    int rightCapture = square + blackPawnCaptureRight;
                    if (isValidSquare(leftCapture) && (square % 8 != 0)) // Not on right edge
                        precompiledPieceVision[PAWN][square][4].push_back(leftCapture);
                    if (isValidSquare(rightCapture) && (square % 8 != 7)) // Not on left edge
                        precompiledPieceVision[PAWN][square][5].push_back(rightCapture);
                }
            }
        }
    }
}
