#ifndef GCODE_PARSER_H
#define GCODE_PARSER_H

#include <string>
#include <vector>
#include <map>

struct GCodeBlock {
    int lineNumber;
    std::map<char, double> words;
    std::string comment;
};

class GCodeParser {
public:
    static bool parseLine(const std::string& line, GCodeBlock& block);
    static std::vector<GCodeBlock> parseFile(const std::string& filename);
private:
    static double parseNumber(const std::string& str);
};

#endif
