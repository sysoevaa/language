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
//
//
//
//
//

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

struct PolizFuncJump : public PolizCell {
    int pos;
};

struct PolizMethodJump : public PolizCell {
    int pos;
};

class DefinitionList {
private:
    std::map<std::string, int> _funcList;
    std::map<std::string, std::map<std::string, int>> _methodList;

public:
    void AddFunc(std::string name, int pos) {
        _funcList[name] = pos;
    }

    void AddMethod(std::string typeName, std::string methodName, int pos) {
        _methodList[typeName][methodName] = pos;
    }
};

class PolizGenerator {
public:
    void Push(Lexeme lex);

private:
    std::vector<PolizCell*> _stack;
    std::vector<int> _last_jmp;

};


#endif //LANGUAGE_POLIZGENERATOR_H
