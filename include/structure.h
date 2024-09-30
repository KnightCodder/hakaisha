#ifndef STRUCTURE_H
#define STRUCTURE_H

#include <iostream>
#include <sstream>
#include <vector>
#include <limits>
#include <string>
#include <atomic>
#include <array>
#include <stack>
#include <thread>
#include <unordered_map>
#include <random>
#include <cstdint>

const int INF = std::numeric_limits<int>::max();

enum PieceType
{
    EMPTY,
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING
};
enum Color
{
    NONE,
    WHITE,
    BLACK
};

struct Piece
{
    PieceType piece;
    Color color;

    Piece(PieceType p = EMPTY, Color c = NONE) : piece(p), color(c) {}

    bool operator==(const Piece &other) const
    {
        return piece == other.piece && color == other.color;
    }

    bool operator!=(const Piece &other) const
    {
        return piece != other.piece || color != other.color;
    }
};

struct Castle
{
    bool WKSC;
    bool WQSC;
    bool BKSC;
    bool BQSC;

    Castle(bool wksc, bool wqsc, bool bksc, bool bqsc) : WKSC(wksc), WQSC(wqsc), BKSC(bksc), BQSC(bqsc) {}
};

// Piece values for evaluation
const int pieceValues[7] = {0, 100, 320, 330, 500, 900, 20000};

// color values form evaluation
const int colorValues[3] = {0, 1, -1};

const Color oppositeColor[3] = {NONE, BLACK, WHITE};

const std::string defaultFen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

struct Move
{
    int from, to;
    Piece promotion;

    Move(int f, int t, Piece p = {EMPTY, NONE}) : from(f), to(t), promotion(p) {}

    std::string toString() const
    {
        // Calculate the file and rank for the from and to squares
        char fromFile = 'a' + (from % 8);
        char fromRank = '1' + (from / 8);
        char toFile = 'a' + (to % 8);
        char toRank = '1' + (to / 8);

        // Handle promotion pieces, if applicable
        char promotionChar = '\0';
        switch (promotion.piece)
        {
        case KNIGHT:
            promotionChar = 'n';
            break;
        case BISHOP:
            promotionChar = 'b';
            break;
        case ROOK:
            promotionChar = 'r';
            break;
        case QUEEN:
            promotionChar = 'q';
            break;
        default:
            promotionChar = '\0'; // No promotion
            break;
        }

        // Build the final move string
        std::string moveStr = std::string() + fromFile + fromRank + toFile + toRank;
        if (promotionChar != '\0') // Add promotion character if applicable
        {
            moveStr += promotionChar;
        }

        return moveStr;
    }
};

class ChessBoard
{
public:
    Piece board[64];
    Color colorToMove;
    std::stack<int> fiftyMoves;
    std::stack<int> enPassantTarget;
    std::stack<std::pair<Move, Piece>> moves;
    std::stack<Castle> castlingRights;
    std::unordered_map<uint64_t, int> positionCount;
    std::array<int, 3> kingPosition;

    ChessBoard(std::string fen = defaultFen)
    {
        std::istringstream iss(fen);
        std::vector<std::string> modified_fen;

        do
        {
            std::string subs;
            iss >> subs;
            modified_fen.push_back(subs);
        } while (iss);

        int file = 0, rank = 7;

        for (char s : modified_fen[0])
        {
            switch ((s))
            {
            case 'p':
                board[rank * 8 + file] = {PAWN, BLACK};
                file++;
                break;
            case 'n':
                board[rank * 8 + file] = {KNIGHT, BLACK};
                file++;
                break;
            case 'b':
                board[rank * 8 + file] = {BISHOP, BLACK};
                file++;
                break;
            case 'r':
                board[rank * 8 + file] = {ROOK, BLACK};
                file++;
                break;
            case 'q':
                board[rank * 8 + file] = {QUEEN, BLACK};
                file++;
                break;
            case 'k':
                board[rank * 8 + file] = {KING, BLACK};
                kingPosition[BLACK] = rank * 8 + file;
                file++;
                break;
            case 'P':
                board[rank * 8 + file] = {PAWN, WHITE};
                file++;
                break;
            case 'N':
                board[rank * 8 + file] = {KNIGHT, WHITE};
                file++;
                break;
            case 'B':
                board[rank * 8 + file] = {BISHOP, WHITE};
                file++;
                break;
            case 'R':
                board[rank * 8 + file] = {ROOK, WHITE};
                file++;
                break;
            case 'Q':
                board[rank * 8 + file] = {QUEEN, WHITE};
                file++;
                break;
            case 'K':
                board[rank * 8 + file] = {KING, WHITE};
                kingPosition[WHITE] = rank * 8 + file;
                file++;
                break;
            case '/':
                file = 0;
                rank--;
                break;
            default:
                int num = s - '0';
                for (int i = file; i < file + num; i++)
                {
                    board[rank * 8 + i] = {EMPTY, NONE};
                }
                file += num;
                break;
            }
        }

        if (modified_fen[1] == "w")
            colorToMove = WHITE;
        else
            colorToMove = BLACK;

        Castle castle = {false, false, false, false};
        for (char s : modified_fen[2])
        {
            switch (s)
            {
            case 'K':
                castle.WKSC = true;
                break;
            case 'Q':
                castle.WQSC = true;
                break;
            case 'k':
                castle.BKSC = true;
                break;
            case 'q':
                castle.BQSC = true;
                break;
            }
        }
        castlingRights.push(castle);

        if (modified_fen[3] == "-")
            enPassantTarget.push(-1);
        else
        {
            enPassantTarget.push((modified_fen[3][0] - 'a') + ((modified_fen[3][1] - '1') * 8));
        }

        fiftyMoves.push(stoi(modified_fen[4]));

        updatePositionCount();
    }

    void updatePositionCount();

    bool isThreefoldRepetition() const;

    void removePositionFromCount();

    void printBoard() const;

    bool checkAt(int index, Color forColor);

    std::vector<Move> generateLegalMoves();

    void makeMove(const Move &move);

    void unmakeMove();

    bool isPawnPromotion(const Move &move)
    {
        return (board[move.from].piece == PAWN && (move.to / 8 == 0 || move.to / 8 == 7));
    }

    int evaluateBoard() const;

    void makeMoveFromUCI(const std::string &moveStr);
};

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

class UCI
{
public:
    ChessEngine engine;

    void uciLoop()
    {
        std::string line;
        while (std::getline(std::cin, line))
        {
            if (line == "uci")
            {
                sendID();
                sendUCIReady();
            }
            else if (line == "isready")
            {
                std::cout << "readyok" << std::endl;
            }
            else if (line.length() >= 8 && line.substr(0, 8) == "position")
            {
                handlePosition(line);
            }
            else if (line.substr(0, 2) == "go")
            {
                handleGo(line);
            }
            else if (line == "stop")
            {
                engine.stop(); // Stop the search
            }
            else if (line == "quit")
            {
                engine.stop(); // Ensure search is stopped before quitting
                break;
            }
        }
    }

    void sendID()
    {
        std::cout << "id name Haki" << std::endl;
        std::cout << "id author Divyansh" << std::endl;
    }

    void sendUCIReady()
    {
        std::cout << "uciok" << std::endl;
    }

    void handlePosition(const std::string &input)
    {
        std::istringstream iss(input);
        std::string token;

        iss >> token; // Skip "position"
        iss >> token;

        if (token == "startpos")
        {
            engine.board = ChessBoard(); // Load standard starting position

            if (iss >> token && token == "moves")
            {
                std::string move;
                while (iss >> move)
                {                    
                    engine.board.makeMoveFromUCI(move); // Apply moves
                }
            }
        }
        else if (token == "fen")
        {
            std::string fen, move;
            while (iss >> token && token != "moves")
            {
                fen += token + " "; // Rebuild FEN string
            }
            engine.board = ChessBoard(fen); // Load the board with the FEN position

            while (iss >> move)
            {
                engine.board.makeMoveFromUCI(move); // Apply moves
            }
        }
        else
        {
            std::cerr << "Error: Invalid position type: " << token << std::endl;
        }
    }

    void handleGo(const std::string & /*input*/)
    {
        int depth = 4;             // Default depth; modify according to input if needed
        engine.startSearch(depth); // Start the search on a separate thread
    }
};

#endif // STRUCTURE_H