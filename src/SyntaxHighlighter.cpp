#include <string>
#include <iostream>
#include <vector>
#include <filesystem>
#include <fstream>
#include <regex>
#include <raylib.h>

namespace fs = std::filesystem;

struct HighlightSpan {
    unsigned long start;
    unsigned long end;
    Color colour;
};

struct GrammarRule {
    std::regex regex;
    Color colour;
};

class Parser {
    // Basic idea:
    // Grammar file -> GrammarRule[]
    // parser (Input file, GrammarRule[]) -> HighlightSpan[]
    // HighlightSpan[] + Input file = Syntax highlight
    public:
    fs::path grammar;
    std::vector<GrammarRule> rules;

    std::vector<HighlightSpan> spans;

    Parser(fs::path grammarPath) {
        grammar = grammarPath;
    }

    HighlightSpan spanAt(long charPos) {
        for (auto span : spans)
            if (span.start <= charPos && span.end >= charPos)
                return span;
        return HighlightSpan{0, 0, WHITE};
    }

    // This function is fucking evil.
    Color stringToColour(std::string str) {
        if (str == "LIGHTGRAY") return LIGHTGRAY ;
        if (str == "GRAY")      return GRAY      ;
        if (str == "DARKGRAY")  return DARKGRAY  ;
        if (str == "YELLOW")    return YELLOW    ;
        if (str == "GOLD")      return GOLD      ;
        if (str == "ORANGE")    return ORANGE    ;
        if (str == "PINK")      return PINK      ;
        if (str == "RED")       return RED       ;
        if (str == "MAROON")    return MAROON    ;
        if (str == "GREEN")     return GREEN     ;
        if (str == "LIME")      return LIME      ;
        if (str == "DARKGREEN") return DARKGREEN ;
        if (str == "SKYBLUE")   return SKYBLUE   ;
        if (str == "BLUE")      return BLUE      ;
        if (str == "DARKBLUE")  return DARKBLUE  ;
        if (str == "PURPLE")    return PURPLE    ;
        if (str == "VIOLET")    return VIOLET    ;
        if (str == "DARKPURPLE")return DARKPURPLE;
        if (str == "BEIGE")     return BEIGE     ;
        if (str == "BROWN")     return BROWN     ;
        if (str == "DARKBROWN") return DARKBROWN ;
        if (str == "WHITE")     return WHITE     ;
        if (str == "BLACK")     return BLACK     ;
        if (str == "BLANK")     return BLANK     ;
        if (str == "MAGENTA")   return MAGENTA   ;
        if (str == "RAYWHITE")  return RAYWHITE  ;
        return BLANK;
    }

    void createRules() {
        rules.clear();
        std::ifstream file;
        file.open(grammar);
        std::string line;

        if (!file.is_open()) {
            std::cout << "FILE WAS NOT OPENED!\n";
            return;
        };
        // read grammar file
        while (std::getline(file, line)) {
            if (line[0] == '#' || line == "") continue;
            // get regex and colour in file
            std::string regexStr;
            std::string colourStr;
            regexStr = line.substr(0, line.find(' '));
            colourStr = line.substr(line.find(' '));
            std::cout << "Found rule: " << regexStr << ": " << colourStr << "\n";
            rules.push_back(GrammarRule {
                std::regex(regexStr),
                stringToColour(colourStr)
            });
        }

        file.close();
    }

    void createHightlights(std::string text)
    {
        for (auto rule : rules)
        {
            // std::cout << "NEW RULE\n";
            typedef std::regex_token_iterator<std::string::iterator> regTokIter;
            // default constructor is eof
            regTokIter eof;

            regTokIter highlights(text.begin(), text.end(), rule.regex);
            while (highlights != eof) {
                // std::cout << ":3 ";
                *highlights++;
                unsigned long start = std::distance(text.begin(), highlights->first);
                auto hlLength = highlights->length();
                unsigned long end = start + (unsigned long)hlLength;

                HighlightSpan span{
                    start, end, rule.colour
                };
                std::cout << "Span from " << span.start << "-" << span.end << std::endl;
                spans.push_back(span);
            }
        }
    }

};
