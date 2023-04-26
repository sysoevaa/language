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

void PolizGenerator::Push(PolizCell *cell) {
    _stack.push_back(cell);
}

void PolizGenerator::MakeExpression(int begin, int end, const std::vector<Lexeme> &_lex) {
    std::vector<Lexeme> oper;
    int st_idx = (int) _stack.size();
    for (int i = begin; i < end; ++i) {
        if (_lex[i].type == "binary") {
            while (!oper.empty() && oper.back().priority >= _lex[i].priority) {
                Push(new PolizOperator(oper.back().string));
                oper.pop_back();
            }
            oper.push_back(_lex[i]);
        } else {
            if (_lex[i].type == "variable") {
                Push(new PolizGet(_lex[i].string, _tid->GetType(_lex[i].string)));
            }
        }
    }
    while (!oper.empty()) {
        Push(new PolizOperator(oper.back().string));
        oper.pop_back();
    }
    std::vector<PolizCell*> current;
    for (int i = st_idx; i < _stack.size(); ++i) {
        if (_stack[i]->type != OPERATOR) {
            current.push_back(_stack[i]);
        } else {
            auto symb = dynamic_cast<PolizOperator*>(_stack[i])->oper;
            auto first = current.back();
            current.pop_back();
            auto second = current.back();
            current.pop_back();
            std::string f, s;
            if (first->type == GET) {
                f = dynamic_cast<PolizGet*>(first)->name;
                f = _tid->GetType(f);
            } else {
                bool type = true;
                for (auto c : dynamic_cast<PolizSymbol*>(first)->string) {
                    if (c < '0' || c > '9') type = false;
                }
                if (type) {
                    f = "float64";
                } else {
                    f = "string";
                }
            }
            if (second->type == GET) {
                s = dynamic_cast<PolizGet*>(first)->type;
            } else {
                bool type = true;
                for (auto c : dynamic_cast<PolizSymbol*>(second)->string) {
                    if (c < '0' || c > '9') type = false;
                }
                if (type) {
                    s = "float64";
                } else {
                    s = "string";
                }
            }
            if (_tid->IsTypeExist(f) == 2 && _tid->IsTypeExist(s) == 2) {
                dynamic_cast<PolizOperator*>(_stack[i])->pos = -1;
            } else {
                dynamic_cast<PolizOperator*>(_stack[i])->pos = _list->GetOverload(f, s, symb);
            }
            current.push_back(new PolizGet("idk", GetResType(f, s, symb)));
        }
    }
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