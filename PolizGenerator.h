#pragma once
#include <vector>
#include "Lexeme.h"

#ifndef LANGUAGE_POLIZGENERATOR_H
#define LANGUAGE_POLIZGENERATOR_H

//COMMANDS:
// ADD
// GET
// WRITE
// JUMP
// FALSE JUMP
// OPERATOR JUMP
//OPERATORS COULD
//TO PROCESS THE OPERATOR WE SHOULD GO THROUGH THE EXPRESSION TWICE:
//  1. FORMING THE DIJKSTRA
//  2. PERFORMING THE DIJKSTRA AND ASSIGNING EACH OPERATOR THEIR CORRESPONDING DEFINITIONS

struct ValType {

};

struct PolizCell {
    int type;
};

struct PolizOperator : PolizCell {
    int address;
    std::string string;
};

struct PolizValuable : PolizCell {
    ValType value;
};

struct PolizJump : PolizCell {
    int address;
};



class PolizGenerator {
public:
    void Push(Lexeme lex);

private:
    std::vector<PolizCell*> _stack;
    std::vector<int> _last_jmp;

};


#endif //LANGUAGE_POLIZGENERATOR_H
