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
    while (in_stream.get(k)) {
        not_divided.push_back(k);
    }
    auto divided = analyser.divide(not_divided);
    divided.emplace_back("END", "END");
    SyntaxAnalyser syntax_analyser(divided);
    try {
        syntax_analyser.program();
    } catch (...) {
        std::cout << "Error found at lexeme: " << syntax_analyser.GetLast().string << "\n";
    }
    if (syntax_analyser.GetLast().string != "END") {
        std::cout << "There are extra symbols at file\n";
    }
}