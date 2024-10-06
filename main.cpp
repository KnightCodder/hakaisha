#include "UCI_protocol.h"

void initializeZobrist();
void compilingPieceVision();
void initializeMLmodel();

int main()
{
    initializeZobrist();
    compilingPieceVision();
    initializeMLmodel();

    UCI uci;
    uci.uciLoop();
    return 0;
}