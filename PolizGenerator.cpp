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
    //gg
    //gggg
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

void PolizGenerator::Push(PolizCell *cell) {
    _stack.push_back(cell);
}

void PolizGenerator::MakeExpression(int begin, int end, const std::vector<Lexeme> &_lex) {
    std::vector<Lexeme> oper
}

std::string PolizGenerator::GetResType(std::string &type1, std::string &type2, std::string &op) {
    if (type1 == "bool") {
        if (type2 == "int32" || type2 == "int64" || type2 == "float32" || type2 == "float64") {
            return type2;
        }
    }
    if (type2 == "bool")  {
        if (type1 == "int32" || type1 == "int64" || type1 == "float32" || type1 == "float64") {
            return type1;
        }
    }
    if (type1 == "int32" || type1 == "int64" || type1 == "float32" || type1 == "float64") {
        if (type2 == "string") return "error";
        if (type2 == "char") return type2;
    }
    if (type1 == "int32") {
        if (type2 == "int32" || type2 == "int64" || type2 == "float32" || type2 == "float64") {
            return type2;
        }
    }
    if (type1 == "int64") {
        if (type2 == "float32" || type2 == "float64") return "float64";
        if (type2 == "int32" || type2 == "int64") {
            return type1;
        }
    }
    if (type1 == "float32") {
        if (type2 == "int32") return type1;
        if (type2 == "int32" || type2 == "int64" || type2 == "float32" || type2 == "float64") {
            return "float64";
        }
    }
    if (type1 == "float64") {
        if (type2 == "int32" || type2 == "int64" || type2 == "float32" || type2 == "float64") {
            return type1;
        }
    }
    if (type1 == "char") {
        if (type2 != "string") return type2;
        return "error";
    }
    if (type1 == "string") {
        if (type2 == "char") return type1;
        return "error";
    }
    return _tid->GetTypeOverload(type2, type1, op);
}