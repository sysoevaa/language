#pragma once
#include <vector>
#include "Lexeme.h"

#ifndef LANGUAGE_POLIZGENERATOR_H
#define LANGUAGE_POLIZGENERATOR_H

struct PolizCell {

};

struct PolizOperator : PolizCell {

};

struct PolizValuable



class PolizGenerator {
public:
    void Push(Lexeme lex);

private:
    std::vector<PolizCell> _stack;
    std::vector<int> _last_jmp;

};


#endif //LANGUAGE_POLIZGENERATOR_H
