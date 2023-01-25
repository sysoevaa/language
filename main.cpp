#include <iostream>
#include "Lexeme.h"
#include <fstream>
#include <cassert>

int main() {
    std::ofstream out_stream("divided.txt");
    std::ifstream in_stream("start_code.txt");
    Analyser analyser;
    std::string string;
    char k;
    while (in_stream.get(k)) {
        string.push_back(k);
    }
    out_stream << string << " " << string.size() << "\n";
    std::vector<Lexeme> v;
    v = analyser.divide(string);
    for (auto a : v) {
        out_stream << '(' << a.type << "," << '"' << a.string << '"' << ")\n";
    }
    return 0;
}