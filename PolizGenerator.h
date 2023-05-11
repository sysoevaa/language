#pragma once
#include <vector>
#include <map>
#include <stack>
#include "Lexeme.h"
#include "TID.h"
#include <fstream>

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
// 0 - все экзеки (инты в строчку) - done
// 1 - глобалки: ТИП ИМЯ - done
// 2 - функции: ТИП ИМЯ КОЛ-ВО ПАРАМЕТРОВ ИМЕНА ПАРАМЕТРОВ
// 3 - мемберы: ИМЯ КЛАССА ТИП МЕМБЕРА ИМЯ МЕМБЕРА
// 4 - методы: ИМЯ КЛАССА ИМЯ МЕТОДА КОЛ-ВО ПАРАМЕТРОВ ИМЕНА ПАРАМЕТРОВ (пренебрегаем возвр типом)
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
    OUTPUT = 11,
    OPERATOR = 12,
    BRACKET = 13,
    SCOPE = 14,
    SCOPECLOSE = 15
};

struct PolizCell {
public:
    PolizCell(Command _type) : type(_type) { }
    Command type;
    virtual void f() {int aboba;}
};

struct PolizAdd : public PolizCell {
    PolizAdd(std::string _name, std::string _type) : PolizCell(ADD), name(_name), type(_type) { }
    std::string name, type;
};

struct PolizGet : public PolizCell {
public:
    PolizGet(const std::string& _name, const std::string& _type) : PolizCell(GET), name(_name), type(_type) { }
    std::string name;
    std::string type;
};

struct PolizWrite : public PolizCell {
    PolizWrite(const std::string& _name) : PolizCell(WRITE), name(_name) { }
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
    PolizSymbol(const std::string& _string) : PolizCell(SYMBOL), string(_string) { }
    std::string string;
};

struct PolizFuncJump : public PolizCell {
    PolizFuncJump(int _pos, int _count, std::string& _type, std::string& _name) :
    PolizCell(FUNCJUMP), pos(_pos), count(_count), type(_type), name(_name) { }
    int pos, count; // position and amount of parameters;
    std::string type, name;
};

struct PolizMethodJump : public PolizCell {
    PolizMethodJump(int _pos, int _count, std::string& _type, std::string& _name) :
    PolizCell(METHODJUMP), pos(_pos), count(_count), type(_type), name(_name) { }
    int pos, count;
    std::string type, name;
};

struct PolizOutput : public PolizCell {
    PolizOutput(int _count) : PolizCell(OUTPUT), count(_count) { }
    int count;
};

struct PolizInput : public PolizCell {
    PolizInput(int _count) : PolizCell(INPUT), count(_count){ }
    int count;
};

struct PolizOperator : public PolizCell {
    PolizOperator(std::string& op, int priority) : PolizCell(OPERATOR), oper(op), prior(priority) {
        if (op == "++" || op == "--" || op == "~") {
            unary = true;
            pos = -1;
        }
    };
    int pos = -1; // -1 if it has already defined
    std::string oper;
    int prior;
    bool unary;
};

struct PolizBracket : public PolizCell {
    PolizBracket(char br) : PolizCell(BRACKET), sym(br) {};
    char sym;
};

struct PolizClose : public PolizCell {
    PolizClose() : PolizCell(SCOPECLOSE){};
};

struct PolizScope : public PolizCell {
    PolizScope() : PolizCell(SCOPE){};
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

class CycleSetter {
public:
    CycleSetter() { }
    void OpenScope(int startPos);
    void CloseScope(int endPos);
    const int GetContinue();
    void PushBreak(PolizJump* breakToPush);

private :
    std::stack<std::pair<int, std::vector<PolizJump*>>> _scopeStack;
};

class PolizGenerator {
public:
    PolizGenerator(TID* tid, DefinitionList* list, CycleSetter* cycleSetter) : _tid(tid), _list(list),
    _cycle_setter(cycleSetter) {}
    void Push(PolizCell* cell);
    void AddFunction(std::string& func_name);
    void AddCast(std::string& type1, std::string& type2);
    void AddMethod(int cnt, std::string& name, std::string& type);
    void AddBreak();
    void AddContinue();
    void OpenCycleScope();
    void CloseCycleScope();
    void MakeExpression();
    void Erase();
    void SetJumps(int begin, int end);
    void print();
    void AddToRes(PolizCell* cell);
    void AddConstructor(std::string& type);
    int GetCurSize() {
        return _res_stack.size();
    }
    std::string GetArrayType(std::string& s);
    std::ofstream out;
    void AddExec(int val);
    void PrintExec();
    void PrintGlobal();
private:
    std::vector<PolizCell*> _stack, _res_stack, _op_stack;
    std::vector<int> _last_jmp, _exec;
    TID* _tid;
    DefinitionList* _list;
    CycleSetter* _cycle_setter;
    std::pair<std::string, int> GetResType(PolizCell* first, PolizCell* second, std::string& op);
};



#endif //LANGUAGE_POLIZGENERATOR_H