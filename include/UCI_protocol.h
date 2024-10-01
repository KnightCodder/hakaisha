#include "engine.h"

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
