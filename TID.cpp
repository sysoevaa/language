#include "TID.h"

TIDElement::TIDElement() {
    _type = "not a function";
    _parent = nullptr;
    _construct = nullptr;
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

void TIDElement::AddVariable(std::string& type, std::string& id) {
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

TIDElement* TIDElement::AddConstructor(std::vector<std::pair<std::string, std::string>> &id) {
    _construct = new TIDElement;
    std::string str = "void";
    _construct->SetType(str);
    for (auto [a, b] : id) {
        _construct->AddVariable(a, b);
    }
    return _construct;
}

TIDElement *TIDElement::AddMethod(std::vector<std::pair<std::string, std::string>> &id, std::string& name, std::string& ret) {
    _methods[name] = new TIDElement;
    _methods[name]->SetType(ret);
    for (auto [type, nm] : id) {
        _methods[name]->AddVariable(type, nm);
    }
    return _methods[name];
}

std::vector<std::string> TIDElement::GetMethodParam(std::string& method_name) {
    std::vector<std::string> vt;
    if (_methods.count(method_name) == 0) throw std::logic_error("Undefined method name");
    for (auto [type, nm] : _methods[method_name]->_id) {
        vt.push_back(type);
    }
    return vt;
}

std::vector<std::string> TIDElement::GetConstructorParam() {
    if (_construct == nullptr) throw std::logic_error("struct does not have any constructor");
    std::vector<std::string> id;
    for (auto [type, name] : _construct->_id) {
        id.push_back(type);
    }
    return id;
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
    if (!ptr) throw std::logic_error("using undefined variable");
    return ptr->GetType(name);
}

void TID::AddFunction(std::string &name, std::vector<std::pair<std::string, std::string>> &formal_parameters, std::string& return_type) {
    if (_functions.count(name) != 0) throw std::logic_error("There are two functions with the same name");
    _functions[name] = new TIDElement;
    _functions[name] ->SetParent(_current_tid);
    _current_tid = _functions[name];
    _functions[name]->SetType(return_type);
    for (auto& [a, b] : formal_parameters) {
        _functions[name]->AddVariable(a, b);
    }
    //_current_tid = _functions[name];
}

std::vector<std::string> TID::GetParameters(std::string &func_name) {
    if (_functions.count(func_name) == 0) throw std::logic_error("Undefined function");
    std::vector<std::string> ret;
    for (auto& [a, b] : _functions[func_name]->GetId()) {
        ret.push_back(b);
    }
    return ret;
}


void TID::AddCast(std::string &type1, std::string &type2, std::string& id) {
    bool find = false;
    for (auto& [a, b] : _cast) {
        if (a.first == type1 && a.first == type2) find = true;
    }
    if (find) throw std::logic_error("There are two casts with the same types");
    auto ptr = new TIDElement;
    ptr ->SetParent(_current_tid);
    ptr ->SetType(type1);
    //ptr->ChangeCast();
    _current_tid = ptr;
    ptr->AddVariable(type2, id);
    _cast.push_back({{type1, type2}, ptr});
}


bool TID::GetCast(std::string &type1, std::string &type2) {
    if (type1 == "void" || type2 == "void") return false;
    //if (IsTypeExist(type1) == 2 && IsTypeExist(type2) == 2) return true;
    for (auto& [a, b] : _cast) {
        if (a.first == type1 && a.second == type2) return true;
    }
    return false;
}

void TID::AddOverload(std::string& id, std::string& type, std::string& oper, std::string& ret){
    auto ptr = _current_tid->AddOverload(type, oper, ret);
    ptr->AddVariable(type, id);
    ptr->SetParent(_current_tid);
    _current_tid = ptr;
}

std::string TID::GetTypeOverload(std::string &type1, std::string &type2, std::string &oper) {
    if (IsTypeExist(type1) == 2 && IsTypeExist(type2) == 2) {
        if (type1 == "int32" || type1 == "int64") {
            if (type2 == "bool" || type2 == "int32" || type2 == "char") return type1;
            if (type1 == "int32" || type2 == "int64" ||
            type2 == "float32" || type2 == "float64") return type2;
            if (type2 == "string") throw std::logic_error("There is no operator " +
                    oper + " for types " + type1 + " and " + type2);
        }
        if (type1 == "string") {
            if (type2 != "char" && oper != "+") throw std::logic_error("There is no operator " +
                    oper + " for types " + type1 + " and " + type2);
            return "string";
        }
        if (type1 == "char") {
            if (type2 == "string") throw std::logic_error("There is no operator " +
                    oper + " for types " + type1 + " and " + type2);
            if (type2 == "float32" || type2 == "float64") return type2;
            return "char";
        }
        if (type1 == "bool") {
            if (type2 == "bool") {
                if (oper == "&&" || oper == "||") return "bool";
                return "int32";
            }
            if (type2 == "string") throw std::logic_error("There is no operator " +
                    oper + " for types " + type1 + " and " + type2);
            return type2;
        }
        if (type1 == "float32" || type1 == "float64") {
            if (type2 == "string")
                throw std::logic_error("There is no operator " +
                    oper + " for types " + type1 + " and " + type2);
            return type1;
        }
    }
    if (IsTypeExist(type1) != 1) std::swap(type1, type2);
    if (_structs.count(type1) == 0) throw std::logic_error("There is no operator " +
    oper + " for types " + type1 + " and " + type2);
    return _structs[type1]->GetOverloadType(type2, oper);
}

std::string TID::GetMember(std::string &type, std::string &name) {
    if (_structs.count(type) == 0) throw std::logic_error("There is no member in this structure with given name");
    return _structs[type]->GetType(name);
}

std::string TID::GetTypeFunction(std::string &name) {
    if (_functions.count(name) == 0) throw std::logic_error("There is no function with used name");
    return _functions[name]->GetType("smth", true);
}

int TID::IsTypeExist(std::string type) {
    int i = 0;
    std::string tmp = "array";
    bool is_array = true;
    for (i = 0; i < 5; ++i) {
        if (tmp[i] != type[i]) {
            is_array = false;
            break;
        }
    }
    if (is_array) {
        while (i < type.size() && type[i] == ' ') ++i;
        std::string type1;
        while (i < type.size()) {
            type1 += type[i];
            ++i;
        }
        type = type1;
    }
    if (type == "char") {
        return 2;
    }
    if (type == "int32") {
        return 2;
    }
    if (type == "int64") {
        return 2;
    }
    if (type == "float32") {
        return 2;
    }
    if (type == "float64") {
        return 2;
    }
    if (type == "string") {
        return 2;
    }
    if (type == "bool") {
        return 2;
    }
    if (type == "void") {
        return 1;
    }
    if (_structs.count(type) == 0) return 0;
    return 1;
}

std::string TID::GetCurrentReturnType() {
    auto ptr = _current_tid;
    while (ptr && ptr->GetType("smth", true) == "not a function") {
        ptr = ptr->GetParent();
    }
    if (ptr) {
        return ptr->GetType("smth", true);
    }
    throw std::logic_error("There is return out of function");
}

void TID::AddStruct(std::string &type) {
    if (_structs.count(type) != 0) throw std::logic_error("There are two structures with the same names");
    _structs[type] = new TIDElement;
    _structs[type] ->SetParent(_current_tid);
    _current_tid = _structs[type];
}

void TID::AddConstructor(std::vector<std::pair<std::string, std::string>> &id) {
    auto ptr = _current_tid->AddConstructor(id);
    ptr->SetParent(_current_tid);
    _current_tid = ptr;
}

bool TID::IsFunctionExist(std::string& id) {
    return _functions.count(id);
}

void TID::AddMethod(std::vector<std::pair<std::string, std::string>> &id, std::string& name, std::string& ret) {
    auto ptr = _current_tid->AddMethod(id, name, ret);
    ptr->SetParent(_current_tid);
    _current_tid = ptr;
}

std::vector<std::string> TID::GetMethodParameters(std::string &struct_name, std::string &method_name) {
    return _structs[struct_name]->GetMethodParam(method_name);
}

std::string TID::GetMethodType(std::string &struct_name, std::string &method_name) {
    return _structs[struct_name]->GetType(method_name, true);
}


std::vector<std::string> TID::GetConstructorParameters(std::string &struct_name) {
    return _structs[struct_name]->GetConstructorParam();
}
