#pragma once
#include <vector>
#include "Lexeme.h"

#ifndef LANGUAGE_POLIZGENERATOR_H
#define LANGUAGE_POLIZGENERATOR_H

//COMMANDS:
// ADD - add variable to the variable list
// GET - get variable's value
// WRITE - re-write variable's value
// JUMP - jump to position in poliz
// FALSE JUMP - if expression is false, jump to position in poliz
// OPERATOR JUMP - jump to the definition of the operator
// RETURN TO CUR POS - jump to current pos in poliz after function end
// SYMBOL - just a symbol, such as ";", "+" (without definition), "="(maybe?), "1", "104"
//TO PROCESS THE OPERATOR WE SHOULD GO THROUGH THE EXPRESSION TWICE:
//  1. FORMING THE DIJKSTRA
//  2. PERFORMING THE DIJKSTRA AND ASSIGNING EACH OPERATOR THEIR CORRESPONDING DEFINITIONS
//Uncertainties : not sure, how to compare types and how to perform casts; how do we count numerical literals..?

struct ValType {

};

struct PolizCell {
    int type;
};

struct PolizAdd : public PolizCell {

};

struct PolizGet : public PolizCell {
    std::string name;
};

struct PolizWrite : public PolizCell {
    std::string name;
};

struct PolizJump : public PolizCell {
    int pos;
};

struct PolizFalseJump : public PolizCell {
    int pos;
};

struct PolizReturn : public PolizCell {

};

struct PolizSymbol : public PolizCell {
    std::string string;
};


class PolizGenerator {
public:
    void Push(Lexeme lex);

private:
    std::vector<PolizCell*> _stack;
    std::vector<int> _last_jmp;

};


#endif //LANGUAGE_POLIZGENERATOR_H
