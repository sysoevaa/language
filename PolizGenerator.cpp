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

void PolizGenerator::AddFunction(std::string &func_name) {
    auto type = _tid->GetTypeFunction(func_name);
    Push(new PolizFuncJump(_list->GetFunc(func_name), _tid->GetParameters(func_name).size(), type));
}

void PolizGenerator::AddCast( std::string &type1,  std::string& type2) {
    Push(new PolizFuncJump(_list->GetCast(type1, type2), 1, type2));
}

void PolizGenerator::AddMethod( std::string &strct,  std::string &method) {
    Push(new PolizOperator((std::string&)".", 0));
    dynamic_cast<PolizOperator*>(_stack.back())->pos = _list->GetMethod(strct, method);
}

void PolizGenerator::Erase() {
    _stack.pop_back();
}

void PolizGenerator::MakeExpression() {
    int begin = _res_stack.size();
    for (int i = 0; i < (int)_stack.size(); ++i) {
        if (_stack[i]->type == BRACKET) {
            auto cur = dynamic_cast<PolizBracket*>(_stack[i]);
            if (cur->sym == '[' || cur->sym == '(') {
                _op_stack.push_back(cur);
            } else {
                while (_op_stack.back()->type != BRACKET && dynamic_cast<PolizBracket*>(_op_stack.back())->sym !=
                                                                     (cur->sym == ')'? '(' : '[')) {
                    _res_stack.push_back(_op_stack.back());
                    _op_stack.pop_back();
                }
                _op_stack.pop_back();
                if (cur->sym == ']') {
                    _res_stack.push_back(new PolizOperator((std::string&) "[]", -1));
                }
            }
        } else if (_stack[i]->type == SYMBOL || _stack[i]->type == GET) {
            _res_stack.push_back(_stack[i]);
        } else if (_stack[i]->type == OPERATOR) {
            auto cur = dynamic_cast<PolizOperator*> (_stack[i]);
            while (dynamic_cast<PolizOperator*>(_op_stack.back())->prior >= cur->prior) {
                if (dynamic_cast<PolizOperator*>(_op_stack.back())->oper != "**") {
                    _res_stack.push_back(_op_stack.back());
                    _op_stack.pop_back();
                } else {
                    if (cur->prior != dynamic_cast<PolizOperator*>(_op_stack.back())->prior) {
                        break;
                    } else {
                        _res_stack.push_back(_op_stack.back());
                        _op_stack.pop_back();
                    }
                }
            }
        } else if (_stack[i]->type == FUNCJUMP) {
            _res_stack.push_back(_stack[i]);
        }
    }
    SetJumps(begin, (int)_res_stack.size());
}

void PolizGenerator::SetJumps(int begin, int end) {
    std::vector<PolizCell*> cur;
    for (int i = begin; i < end; ++i) {
        if (_res_stack[i]->type == GET || _res_stack[i]->type == SYMBOL) {
            cur.push_back(_res_stack[i]);
        } else if (_res_stack[i]->type == FUNCJUMP) {
            for (int j = 0; j < dynamic_cast<PolizFuncJump*>(_res_stack[i])->count; ++j) {
                cur.pop_back();
            }
            cur.push_back(new PolizGet("RETURN FROM FUNCTION", dynamic_cast<PolizFuncJump*>(_res_stack[i])->type));
        } else if (_res_stack[i]->type == OPERATOR) {
            auto first = cur.back(); cur.pop_back();
            auto second = cur.back(); cur.pop_back();
            if (dynamic_cast<PolizOperator*>(_res_stack[i])->oper == ".") {
                _tid->GetMember(dynamic_cast<PolizGet *>(second)->type, dynamic_cast<PolizGet *>(first)->name);
                break;
            }
            auto [res, jmp] = GetResType(second, first, dynamic_cast<PolizOperator*>(_res_stack[i])->oper);
            cur.push_back(new PolizGet("VALUE FROM EXPR)",res));
            dynamic_cast<PolizOperator*>(_res_stack[i])->pos = jmp;

        } else if (_res_stack[i]->type == METHODJUMP) {
            for (int j = 0; j < dynamic_cast<PolizMethodJump*>(_res_stack[i])->count; ++j) {
                cur.pop_back();
            }
            cur.push_back(new PolizGet("RETURN FROM METHOD", dynamic_cast<PolizMethodJump*>(_res_stack[i])->type));
        }
    }
}

std::pair<std::string, int> PolizGenerator::GetResType(PolizCell *first, PolizCell *second, std::string &op) {
    std::string type1, type2;
    if (first->type == SYMBOL) {
        auto val = dynamic_cast<PolizSymbol*>(first)->string;
        bool num = true;
        for (auto c : val) {
            if (c >= '9' || c <= '0') {
                num = false;
            }
        }
        if (num) {
            type1 = "float64";
        } else {
            type1 = "string";
        }
    } else {
        type1 = dynamic_cast<PolizGet*>(first)->type;
    }
    if (second->type == SYMBOL) {
        auto val = dynamic_cast<PolizSymbol*>(second)->string;
        bool num = true;
        for (auto c : val) {
            if (c >= '9' || c <= '0') {
                num = false;
            }
        }
        if (num) {
            type2 = "float64";
        } else {
            type2 = "string";
        }
    } else {
        type2 = dynamic_cast<PolizGet*>(first)->type;
    }
    if (type1 == "bool") {
        if (type2 == "int32" || type2 == "int64" || type2 == "float32" || type2 == "float64") {
            return {type2, -1};
        }
    }
    if (type2 == "bool")  {
        if (type1 == "int32" || type1 == "int64" || type1 == "float32" || type1 == "float64") {
            return {type1, -1};
        }
    }
    if (type1 == "int32" || type1 == "int64" || type1 == "float32" || type1 == "float64") {
        if (type2 == "string") return {"error", -1};
        if (type2 == "char") return {type2, -1};
    }
    if (type1 == "int32") {
        if (type2 == "int32" || type2 == "int64" || type2 == "float32" || type2 == "float64") {
            return {type2, -1};
        }
    }
    if (type1 == "int64") {
        if (type2 == "float32" || type2 == "float64") return {"float64", -1};
        if (type2 == "int32" || type2 == "int64") {
            return {type1, -1};
        }
    }
    if (type1 == "float32") {
        if (type2 == "int32") return {type1, -1};
        if (type2 == "int32" || type2 == "int64" || type2 == "float32" || type2 == "float64") {
            return {"float64", -1};
        }
    }
    if (type1 == "float64") {
        if (type2 == "int32" || type2 == "int64" || type2 == "float32" || type2 == "float64") {
            return {type1, -1};
        }
    }
    if (type1 == "char") {
        if (type2 != "string") return {type2, -1};
        return {"error", -1};
    }
    if (type1 == "string") {
        if (type2 == "char") return {type1, -1};
        return {"error", -1};
    }
    return {_tid->GetTypeOverload(type2, type1, op), _list->GetOverload(type2, type1, op)}; // maybe here is a mistake
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