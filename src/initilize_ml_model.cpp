#include "structure.h"
#include <fstream>

const std::string dataFileName = "models/modelData.txt";

extern std::array<double, 67> coefficients;
extern double intercept;

void initializeMLmodel()
{
    std::ifstream infile(dataFileName);
    std::string line;
    std::getline(infile, line);

    std::istringstream iss(line);
    double value;
    int it = 0;

    while (iss >> value)
    {
        if (it == 67)
        {
            intercept = value;
            break;
        }

        coefficients[it] = value;
        it++;
    }
}