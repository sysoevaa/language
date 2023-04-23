#pragma once
#include <vector>
#include <map>
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
//
//Вывод структуры: выводим каждое поле класса видом
// поле: знач
// поле:
//  поле: знач
//  поле: знач
//
//Ввод полей: можно теоретически что-то похожее сделать, типо вывести в консоль название поля и двоеточие, ожидать ввод
//
//Важно!! у нас, когда считается выражение, то значение какой-то части мы как храним????
//
//типо (a + b) / c
// а + b - перегруженная хрень какая-то, вернула нам значчение, что мы с этим значением делайим?? как обработать его???
//
//типо мы фигачим его тупо туда и потом типо такие берем его и тупо это того под деление его подкладываем вот и типо оно
//хранится где-то и типо потом удаляем наверное это значение или же какой-то указатель или тмп переменную хуячим
//
//как проверить все поля класса при создании переменной с его типом? храним ли мы это где-то отдельно или же эта хуевинка
//где-то через констракт фигчится?? кста да констрактом же можно запихнуть в тид все и типо связать вот этим нодиком со
//структурой воот
//
//
//ADD - 1
//GET -
//
//
//

enum Command {
    ADD = 1,
    GET = 2,
    WRITE = 3,
    JUMP = 4,
    FALSEJUMP = 5,
    RETURN = 6,
    SYMBOL = 7,
    FUNCJUMP = 8,
    METHODJUMP = 9,
    INPUT = 10,
    OUTPUT = 11
};


struct ValType {

};

struct PolizCell {
    PolizCell(Command _type) : type(_type) { }
    Command type;
};

struct PolizAdd : public PolizCell {
    PolizAdd() : PolizCell(ADD) { }
};

struct PolizGet : public PolizCell {
    PolizGet(std::string _name) : PolizCell(GET), name(_name) { }
    std::string name;
};

struct PolizWrite : public PolizCell {
    PolizWrite(std::string _name) : PolizCell(WRITE), name(_name) { }
    std::string name;
};

struct PolizJump : public PolizCell {
    PolizJump(int _pos) : PolizCell(JUMP), pos(_pos) { }
    int pos;
};

struct PolizFalseJump : public PolizCell {
    PolizFalseJump(int _pos) : PolizCell(FALSEJUMP), pos(_pos) { }
    int pos;
};

struct PolizReturn : public PolizCell {
    PolizReturn() : PolizCell(RETURN) { }
};

struct PolizSymbol : public PolizCell {
    PolizSymbol(std::string _string) : PolizCell(SYMBOL), string(_string) { }
    std::string string;
};

struct PolizFuncJump : public PolizCell {
    PolizFuncJump(int _pos) : PolizCell(FUNCJUMP), pos(_pos) { }
    int pos;
};

struct PolizMethodJump : public PolizCell {
    PolizMethodJump(int _pos) : PolizCell(METHODJUMP), pos(_pos) { }
    int pos;
};

struct PolizOutput : public PolizCell {
    PolizOutput() : PolizCell(OUTPUT) { }
};

struct PolizInput : public PolizCell {
    PolizInput() : PolizCell(INPUT) { }
};

struct OverloadParameters {
    OverloadParameters(std::string _type1, std::string _type2, std::string _op) :
    type1(_type1), type2(_type2), op(_op) { }
    std::string type1;
    std::string type2;
    std::string op;
};

class DefinitionList {
private:
    std::map<std::string, int> _funcList;
    std::map<std::string, std::map<std::string, int>> _methodList;
    std::map<std::pair<std::string, std::pair<std::string, std::string>>, int> _overloadList;
    std::map<std::pair<std::string, std::string>, int> _castList;

public:
    void AddFunc(std::string name, int pos);
    void AddMethod(std::string typeName, std::string methodName, int pos);
    void AddOverload(std::string type1, std::string type2, std::string op, int pos);
    void AddCast(std::string type1, std::string type2, int pos);

    const int GetFunc(std::string name);
    const int GetMethod(std::string typeName, std::string methodName);
    const int GetOverload(std::string type1, std::string type2, std::string op);
    const int GetCast(std::string type1, std::string type2);

};

class PolizGenerator {
public:
    void Push(Lexeme lex);

private:
    std::vector<PolizCell*> _stack;
    std::vector<int> _last_jmp;

};


#endif //LANGUAGE_POLIZGENERATOR_H
