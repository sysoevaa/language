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

void PolizGenerator::AddFunction(const std::string &func_name) {
    Push(new PolizFuncJump(_list->GetFunc(func_name)));
}

void PolizGenerator::AddCast(const std::string &type1, const std::string& type2) {
    Push(new PolizFuncJump(_list->GetCast(type1, type2)));
}

void PolizGenerator::AddMethod(const std::string &strct, const std::string &method) {
    Push(new PolizOperator((std::string&)".", -1));
    dynamic_cast<PolizOperator*>(_stack.back())->pos = _list->GetMethod(strct, method);
}

void PolizGenerator::Erase() {
    _stack.pop_back();
}

void PolizGenerator::MakeExpression() {

}

void CycleSetter::OpenScope(int startPos) {
    _scopeStack.push({startPos, std::vector<PolizJump*>()});
}

void CycleSetter::CloseScope(int endPos) {
    std::vector<PolizJump*> tmp = _scopeStack.top().second;
    for (auto i : tmp) {
        i->pos = endPos;
    }
    _scopeStack.pop();
}

const int CycleSetter::GetContinue() {
    return _scopeStack.top().first;
}

void CycleSetter::PushBreak(PolizJump *breakToPush) {
    _scopeStack.top().second.push_back(breakToPush);
}