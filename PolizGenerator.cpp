//
// Created by А ну да on 14.04.2023.
//

// вместо operator ref != -1 methodjump как обычно

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

void PolizGenerator::AddFunction(std::string &func_name) {
    auto type = _tid->GetTypeFunction(func_name);
    Push(new PolizFuncJump(_list->GetFunc(func_name), _tid->GetParameters(func_name).size(), type, func_name));
}

void PolizGenerator::AddCast( std::string &type1, std::string& type2) {
    Push(new PolizFuncJump(_list->GetCast(type1, type2), 1, type2, type1)); // may be should be another name
}

void PolizGenerator::AddMethod(int cnt, std::string& name, std::string& type) {
    _stack.push_back(new PolizMethodJump(-1, cnt, type, name));
}

void PolizGenerator::AddBreak() {
    PolizJump* breaky = new PolizJump(0);
    _res_stack.push_back(breaky);
    _cycle_setter->PushBreak(breaky);
}

void PolizGenerator::AddContinue() {
    _res_stack.push_back(new PolizJump(_cycle_setter->GetContinue()));
}

void PolizGenerator::OpenCycleScope() {
    _res_stack.push_back(new PolizScope);
    _cycle_setter->OpenScope(this->GetCurSize());
}

void PolizGenerator::CloseCycleScope() {
    _cycle_setter->CloseScope(this->GetCurSize());
    _res_stack.push_back(new PolizClose);
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
                while (_op_stack.back()->type != BRACKET  || dynamic_cast<PolizBracket*>(_op_stack.back())->sym !=
                                                                     (cur->sym == ')'? '(' : '[')) {
                    _res_stack.push_back(_op_stack.back());
                    _op_stack.pop_back();
                }
                _op_stack.pop_back();
                if (cur->sym == ']') {
                    std::string br = "[]";
                    _res_stack.push_back(new PolizOperator(br, -1));
                }
            }
        } else if (_stack[i]->type == SYMBOL || _stack[i]->type == GET ||
        _stack[i]->type == ADD || _stack[i]->type == WRITE) {
            _res_stack.push_back(_stack[i]);
        } else if (_stack[i]->type == OPERATOR) {
            auto cur = dynamic_cast<PolizOperator*> (_stack[i]);
            while (_op_stack.size() > 0 && _op_stack.back()->type == OPERATOR &&
            dynamic_cast<PolizOperator*>(_op_stack.back())->prior <= cur->prior && !cur->unary) {
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
            _op_stack.push_back(cur);
        } else if (_stack[i]->type == FUNCJUMP || _stack[i]->type == METHODJUMP) {
            _res_stack.push_back(_stack[i]);
        }
    }
    while (_op_stack.size() != 0) {
        _res_stack.push_back(_op_stack.back());
        _op_stack.pop_back();
    }
    SetJumps(begin, (int)_res_stack.size());
    _stack.clear();
}

void PolizGenerator::SetJumps(int begin, int end) {
    std::vector<PolizCell*> cur;
    for (int i = begin; i < end; ++i) {
        if (_res_stack[i]->type == GET || _res_stack[i]->type == SYMBOL ||
        _res_stack[i]->type == ADD || _res_stack[i]->type == WRITE) {
            cur.push_back(_res_stack[i]);
        } else if (_res_stack[i]->type == FUNCJUMP) {
            for (int j = 0; j < dynamic_cast<PolizFuncJump*>(_res_stack[i])->count; ++j) {
                cur.pop_back();
            }
            cur.push_back(new PolizGet("RETURN FROM FUNCTION", dynamic_cast<PolizFuncJump*>(_res_stack[i])->type));
        } else if (_res_stack[i]->type == OPERATOR) {
            auto cell = dynamic_cast<PolizOperator*>(_res_stack[i]);
            if (cell->unary) {
                auto first = cur.back(); cur.pop_back();
                cur.push_back(first);
            } else if (cell->oper == "[]") {
                auto first = cur.back();
                cur.pop_back();
                auto second = cur.back();
                cur.pop_back();
                auto type = GetArrayType(dynamic_cast<PolizGet*>(second)->type);
                cur.push_back(new PolizGet("ELEMENT OF ARRAY", type));
            } else if (cell->oper != "=") {
                auto first = cur.back();
                cur.pop_back();
                auto second = cur.back();
                cur.pop_back();
                if (cell->oper == ".") {
                    auto type = _tid->GetMember(dynamic_cast<PolizGet *>(second)->type, dynamic_cast<PolizGet *>(first)->name);
                    cur.push_back(new PolizGet("MEMBER", type));
                    continue;
                }
                auto [res, jmp] = GetResType(second, first, dynamic_cast<PolizOperator *>(_res_stack[i])->oper);
                cur.push_back(new PolizGet("VALUE FROM EXPR)", res));
                if (jmp != -1) {
                    delete _res_stack[i];
                    _res_stack[i] = new PolizMethodJump(jmp, 1, dynamic_cast<PolizGet*>(second)->type, cell->oper);
                }
                //dynamic_cast<PolizOperator *>(_res_stack[i])->pos = jmp;
            }
        } else if (_res_stack[i]->type == METHODJUMP) {
            for (int j = 0; j < dynamic_cast<PolizMethodJump*>(_res_stack[i])->count; ++j) {
                cur.pop_back();
            }
            auto method = dynamic_cast<PolizMethodJump*>(_res_stack[i]);
            if (method->name == "CONSTRUCTOR") {
                int aboba;
            }
            method->pos = _list->GetMethod(dynamic_cast<PolizGet*>(cur.back())->type, method->name);
            cur.pop_back();
            cur.push_back(new PolizGet("RETURN FROM METHOD", dynamic_cast<PolizMethodJump*>(_res_stack[i])->type));
        }
    }
}

std::string PolizGenerator::GetArrayType(std::string &s) {
    int i = 0;
    std::string tmp = "array";
    for (i = 0; i < 5; ++i) {
        if (tmp[i] != s[i]) throw std::logic_error("array expected");
    }
    while (i < s.size() && s[i] == ' ') ++i;
    std::string type;
    while (i < s.size()) {
        type += s[i];
        ++i;
    }
    return type;
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

void PolizGenerator::print() {
    PrintExec();
    PrintGlobal();
    PrintFunc();
    PrintCast();
    PrintMembers();
    PrintMethods();
    for (auto cell : _res_stack) {
        if (cell->type == OPERATOR) {
            out << "operator " << dynamic_cast<PolizOperator*>(cell)->oper << ' ';
            int ref = dynamic_cast<PolizOperator*>(cell)->pos;
            if (ref != -1) out << "methodjump" << ref;
            out << '\n';
        } else if (cell->type == GET) {
            out << "get " << dynamic_cast<PolizGet*>(cell)->name << '\n';
        } else if (cell->type == JUMP) {
            out << "jump " << dynamic_cast<PolizJump*>(cell)->pos << '\n';
        } else if (cell->type == FALSEJUMP) {
            out << "falsejump " << dynamic_cast<PolizFalseJump*>(cell)->pos << '\n';
        } else if (cell->type == FUNCJUMP) {
            out << "funcjump " << dynamic_cast<PolizFuncJump*>(cell)->pos << '\n';
        } else if (cell->type == METHODJUMP) {
            out << "methodjump " << dynamic_cast<PolizMethodJump*>(cell)->pos << '\n';
        } else if (cell->type == SYMBOL) {
            out << "symbol " << dynamic_cast<PolizSymbol*>(cell)->string << '\n';
        } else if (cell->type == INPUT) {
            out << "get " << dynamic_cast<PolizInput*>(cell)->count << "\n";
        } else if (cell->type == OUTPUT) {
            out << "output " << dynamic_cast<PolizOutput*>(cell)->count << "\n";
        } else if (cell->type == ADD) {
            out << "add " << dynamic_cast<PolizAdd*>(cell)->name << '\n';
        } else if (cell->type == WRITE) {
            out << "rewrite " << dynamic_cast<PolizWrite*>(cell)->name << '\n';
        }
        out.flush();
    }
}

void PolizGenerator::AddToRes(PolizCell* cell) {
    _res_stack.push_back(cell);
}

void PolizGenerator::AddConstructor(std::string &type) {
    _stack.push_back(new PolizMethodJump(_list->GetMethod(type, (std::string&) "CONSTRUCTOR"),
                                         _tid->GetConstructorParameters(type).size(),
                                         (std::string&)"void", (std::string&) "CONSTRUCTOR"));
}

void PolizGenerator::AddExec(int val) {
    _exec.push_back(val);
}

void PolizGenerator::PrintExec() {
    out << _exec.size() << '\n';
    for (auto el : _exec) {
        out << el << ' ';
    }
    out << "\n" << std::endl;
}

void PolizGenerator::PrintGlobal() {
    auto ptr = _tid->GetGlobal();
    out << ptr->GetId().size() << '\n';
    for (auto [id, type] : ptr->GetId()) {
        out << type << ' ' << id << "\n";
    }
    out << std::endl;
}

void PolizGenerator::PrintFunc() {
    auto mp = _tid->GetFunc();
    out << mp.size() + _tid->GetCast().size() << '\n';
    for (auto [name, ptr] : mp) {
        out << name << ' ' << ptr->GetType("aboba", true) << '\n';
        out << ptr->GetId().size() << '\n';
        for (auto [id, type] : ptr->GetId()) {
            out << type << ' ' << id << '\n';
        }
    }
    out << std::endl;
}

void PolizGenerator::PrintMethods() {
    auto st = _tid->GetStructs();
    out << st.size() << '\n';
    for (auto [name, ptr] : st) {
        out << name << '\n';
        for (auto [m_name, m_ptr] : ptr->GetMethods()) {
            out << m_name << "\n " << m_ptr->GetId().size();
            for (auto [id, type] : m_ptr->GetId()) {
                out << id << '\n';
            }
        }
    }
    out.flush();
}

void PolizGenerator::PrintMembers() {
    auto st = _tid->GetStructs();
    out << st.size() << '\n';
    for (auto [name, ptr] : st) {
        out << name << '\n';
            for (auto [id, type] : ptr->GetId()) {
                out << type << ' ' << id << '\n';
            }
    }
    out.flush();
}

void PolizGenerator::PrintCast() {
    auto vct = _tid->GetCast();
    for (auto [v, ptr] : vct) {
        out << ' ' << v.second << " " <<   v.first + v.second << "\n1\n";
        for (auto [id, type] : ptr->GetId()) {
            out << type << ' ' << id << '\n';
        }
    }
    out << std::endl;
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