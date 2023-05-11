#include <iostream>
#include "Lexeme.h"
#include <fstream>
#include "Syntax.h"
#include <cassert>
#include "Executive.h"

int main() {
    std::ofstream out_stream("divided.txt");
    std::ifstream in_stream("start_code.txt");
    Analyser analyser;
    std::string not_divided;
    char k;
    //
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
    syntax_analyser.GetGen()->out.open("poliz.txt");
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

    syntax_analyser.GetGen()->print();
    syntax_analyser.GetGen()->out.close();

    Executive FINALLY;
    std::ifstream f;
    f.open("poliz.txt");
    FINALLY.ReadPoliz(f);
    f.close();
    FINALLY.ExecuteProgram();
    std::cout << "Everything is OK";
}