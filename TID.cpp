#include "TID.h"

TIDElement::TIDElement() {
    _type = "not a function";
    _parent = nullptr;
}

void TIDElement::SetParent(TIDElement *parent) {
    _parent = parent;
}

void TIDElement::SetType(std::string &id) {
    _type = id;
}

std::string TIDElement::GetType(std::string id, bool func) {
    if (func) return _type;
    if (_id.count(id) == 0) return "undefined variable";
    return _id[id];
}

void TIDElement::AddVariable(std::string id, std::string type) {
    _id[id] = type;
}

TIDElement *TIDElement::GetParent() {
    return _parent;
}

std::map<std::string, std::string> TIDElement::GetId(){
    return _id;
}

TIDElement* TIDElement::AddOverload(std::string& type, std::string& oper, std::string& ret) {
    if (_overload.count({type, oper}) != 0) throw std::logic_error("There are two overload for same operators and same type");
    _overload[{type, oper}] = new TIDElement;
    _overload[{type, oper}]->SetType(ret);
    return _overload[{type, oper}];
}

std::string TIDElement::GetOverloadType(std::string& type, std::string& oper) {
    if (_overload.count({type, oper}) == 0) throw std::logic_error("There is no such overload function");
    return _overload[{type, oper}]->GetType("smth", true);
}


TID::TID() {
    _current_tid = new TIDElement;
}

void TID::OpenScope() {
    auto ptr = new TIDElement;
    ptr->SetParent(_current_tid);
    _current_tid = ptr;
}

void TID::CloseScope() {
    _current_tid = _current_tid->GetParent();
}

void TID::AddVariable(std::string &type, std::string &name) {
    _current_tid->AddVariable(type, name);
}

std::string TID::GetType(std::string &name) {
    auto ptr = _current_tid;
    while (ptr && ptr->GetType(name) == "undefined variable") {
        ptr = ptr->GetParent();
    }
    return ptr->GetType(name);
}

void TID::AddFunction(std::string &name, std::vector<std::pair<std::string, std::string>> &formal_parameters, std::string& return_type) {
    if (_functions.count(name) != 0) throw std::logic_error("There are two functions with the same name");
    _functions[name] = new TIDElement;
    _functions[name]->SetType(return_type);
    for (auto& [a, b] : formal_parameters) {
        _functions[name]->AddVariable(a, b);
    }
}

std::vector<std::string> TID::GetParameters(std::string &func_name) {
    if (_functions.count(func_name) == 0) throw std::logic_error("Undefined function");
    std::vector<std::string> ret;
    for (auto& [a, b] : _functions[func_name]->GetId()) {
        ret.push_back(b);
    }
    return ret;
}


void TID::AddCast(std::string &type1, std::string &type2) {
    bool find = false;
    for (auto& [a, b] : _cast) {
        if (a.first == type1 && a.first == type2) find = true;
    }
    if (find) throw std::logic_error("There are two casts with the same types");
    auto ptr = new TIDElement;
    ptr ->SetParent(_current_tid);
    _cast.push_back({{type1, type2}, ptr});
}


bool TID::GetCast(std::string &type1, std::string &type2) {
    for (auto& [a, b] : _cast) {
        if (a.first == type1 && a.second == type2) return true;
    }
    return false;
}

void TID::AddOverload(std::string& type, std::string& oper, std::string& ret){
    auto ptr = _current_tid->AddOverload(type, oper, ret);
    ptr->SetParent(_current_tid);
    _current_tid = ptr;
}

std::string TID::GetTypeOverload(std::string &type1, std::string &type2, std::string &oper) {
    if (_functions.count(type1) == 0) throw std::logic_error("No such overload function");
    return _functions[type1]->GetOverloadType(type2, oper);
}

std::string TID::GetMember(std::string &type, std::string &name) {
    if (_structs.count(type) == 0) throw std::logic_error("There is no member in this structure with given name");
    return _structs[type]->GetType(name);
}

std::string TID::GetTypeFunction(std::string &name) {
    if (_functions.count(name) == 0) throw std::logic_error("There is no function with used name");
    return _functions[name]->GetType("smth", true);
}

bool TID::IsTypeExist(std::string &type) {
    if (type == "char") {
        return true;
    }
    if (type == "int32") {
        return true;
    }
    if (type == "int64") {
        return true;
    }
    if (type == "float32") {
        return true;
    }
    if (type == "float64") {
        return true;
    }
    if (type == "string") {
        return true;
    }
    if (type == "bool") {
        return true;
    }
    return _structs.count(type);
}

std::string TID::GetCurrentReturnType() {
    auto ptr = _current_tid;
    while (ptr && ptr->GetType("smth", true) == "not a function") {
        ptr = ptr->GetParent();
    }
    if (ptr) return ptr->GetType("smth", true);
    throw std::logic_error("There is return out of function");
}

void TID::AddStruct(std::string &type) {
    if (_structs.count(type) != 0) throw std::logic_error("There are two structures with the same names");
    _structs[type] = new TIDElement;
    _structs[type] ->SetParent(_current_tid);
    _current_tid = _structs[type];
}
