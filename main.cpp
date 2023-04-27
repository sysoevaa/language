#include <iostream>
#include "Lexeme.h"
#include <fstream>
#include "Syntax.h"
#include <cassert>

int main() {
    std::ofstream out_stream("divided.txt");
    std::ifstream in_stream("start_code.txt");
    Analyser analyser;
    std::string not_divided;
    char k;

    // comment
    while (in_stream.get(k)) {
        not_divided.push_back(k);
    }
    auto divided = analyser.divide(not_divided);
    if (divided.back().string == "ERR") {
        std::cout << "Unexpected symbols in text were found";
        return 1;
    }
    divided.emplace_back("END", "END", divided.back().num + 1);
    SyntaxAnalyser syntax_analyser(divided);
    try {
        syntax_analyser.program();
    } catch (std::exception& e) {
        std::cout << "Error found at lexeme: \"" << syntax_analyser.GetLast().string << "\" in line " << syntax_analyser.GetLast().num << "\n";
        std::cout << e.what();
        return 1;
    }
    if (syntax_analyser.GetLast().string != "END") {
        std::cout << "There are extra symbols at file\n";
        return 1;
    }
    std::cout << "Everything is OK";
}