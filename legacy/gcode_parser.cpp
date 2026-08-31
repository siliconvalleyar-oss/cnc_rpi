#include "gcode_parser.h"
#include <fstream>
#include <sstream>

bool GCodeParser::parseLine(const std::string& line, GCodeBlock& block) {
    block.words.clear();
    std::string cleanLine = line;
    size_t commentPos = cleanLine.find(';');
    if(commentPos != std::string::npos) {
        block.comment = cleanLine.substr(commentPos + 1);
        cleanLine = cleanLine.substr(0, commentPos);
    }
    
    std::istringstream iss(cleanLine);
    std::string token;
    while(iss >> token) {
        if(token.empty()) continue;
        char code = token[0];
        double value = parseNumber(token.substr(1));
        block.words[code] = value;
    }
    return !block.words.empty();
}

double GCodeParser::parseNumber(const std::string& str) {
    try { return std::stod(str); } catch(...) { return 0.0; }
}

std::vector<GCodeBlock> GCodeParser::parseFile(const std::string& filename) {
    std::vector<GCodeBlock> blocks;
    std::ifstream file(filename);
    std::string line;
    while(std::getline(file, line)) {
        GCodeBlock block;
        if(parseLine(line, block)) blocks.push_back(block);
    }
    return blocks;
}
