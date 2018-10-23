#include "header.h"
#include <fstream>
#include <sstream>

Input::Input(std::istream& stream)
{
    ReadHeader(stream);
    ReadValues(stream);
}

Input Input::FromFile(const std::string& filename)
{
    std::ifstream infile(filename);
    return Input(infile);
}

void Input::ReadHeader(std::istream& stream)
{
    std::string line;
    std::getline(stream, line);
    const auto spacePosition = line.find(' ');
    numberOfRows = std::stoi(line.substr(0, spacePosition));
    numberOfCols = std::stoi(line.substr(spacePosition + 1, std::string::npos));
}

void Input::ReadValues(std::istream& stream)
{
    std::string line;
    while(std::getline(stream, line) || values.size() < size_t(numberOfRows)) {
        values.push_back(line);
    }
}


std::ostream& operator<<(std::ostream& os, const Input& input)
{
    os << "rows: " << input.numberOfRows << ", cols: " << input.numberOfCols << "\n";
    for (const auto& line : input.values) {
        os << line << std::endl;
    }

    return os;
}
