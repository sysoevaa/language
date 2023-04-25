//
// Created by А ну да on 14.04.2023.
//

#include "PolizGenerator.h"

void DefinitionList::AddFunc(std::string name, int pos) {
    _funcList[name] = pos;
}

void DefinitionList::AddMethod(std::string typeName, std::string methodName, int pos) {
    _methodList[typeName][methodName] = pos;
}

void DefinitionList::AddOverload(std::string type1, std::string type2, std::string op, int pos) {
    OverloadParameters parameter(type1, type2, op);
    _overloadList[{op, {type1, type2}}] = pos;
    _overloadList[{op, {type2, type1}}] = pos;
}

void DefinitionList::AddCast(std::string type1, std::string type2, int pos) {
    _castList[{type1, type2}] = pos;
    _castList[{type2, type1}] = pos;
}

const int DefinitionList::GetFunc(std::string name) {
    return _funcList[name];
}

const int DefinitionList::GetCast(std::string type1, std::string type2) {
    return _castList[{type1, type2}];
}

const int DefinitionList::GetMethod(std::string typeName, std::string methodName) {
    return _methodList[typeName][methodName];
}

const int DefinitionList::GetOverload(std::string type1, std::string type2, std::string op) {
    return _overloadList[ { op, { type1, type2 } } ];
}

void PolizGenerator::Push(const Lexeme& lex) {
    PolizCell* ptr;
    //if (lex.type == "")
}

void PolizGenerator::Push(PolizCell *cell) {
    _stack.push_back(cell);

}

void PolizGenerator::MakeExpression(int begin, int end, const std::vector<Lexeme> &_lex) {
    std::vector<Lexeme> oper;
    for (int i = begin; i < end; ++i) {
        if (_lex[i].type == "binary") {
            while (!oper.empty() && oper.back().priority >= _lex[i].priority) {
                Push(oper.back());
                oper.pop_back();
            }
            oper.push_back(_lex[i]);
        } else {
            Push(_lex[i]);
        }
    }
    while (!oper.empty()) {
        Push(oper.back());
        oper.pop_back();
    }
}
