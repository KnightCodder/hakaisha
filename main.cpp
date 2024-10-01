#include "UCI_protocol.h"

void initializeZobrist();
void compilingPieceVision();

int main()
{
    initializeZobrist();
    compilingPieceVision();

    UCI uci;
    uci.uciLoop();
    return 0;
}