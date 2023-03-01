#include "TID.h"

void TIDElement::AddVariable(std::string id, std::string type) {
    _id[id] = type;
}

std::string TIDElement::GetType(std::string id) {
    if (_id.count(id) == 0) {
        return (std::string) "UNDEFINED";
    }
    return _id[id];
}

TIDElement* TIDElement::GetParent() {
    return parent;
}

TIDElement* TIDElement::NewScope() {
    _scopes.push_back(new TIDElement);
    _scopes.back()->parent = this;
    return _scopes.back();
}

TIDElement::TIDElement() {
    parent = nullptr;
}

std::vector<std::string> FunctionDef::GetParameters() {
    return _formal_parameters;
}

FunctionDef::FunctionDef() {
    parent = nullptr;
}

std::string FunctionDef::GetType() {
    return _type;
}

FunctionDef* StructDef::AddMethod(std::string id) {
    if (_methods.count(id) != 0) throw std::logic_error("There are two same methods"
                                                         " in one structure");
    return _methods[id] = new FunctionDef;
}

void StructDef::AddMember(std::string id, std::string type) {
    _members->AddVariable(id, type);
}

std::string StructDef::FindMember(std::string id) {
    return _members->GetType(id);
}

FunctionDef* StructDef::FindFunction(std::string id) {
    if (_methods.count(id) == 0) return nullptr;
    return _methods[id];
}

TID::TID() {
    global = new TIDElement;
    _current_tid = global;
}

std::string TID::GetType(std::string id) {
    auto find_type = _current_tid;
    while (find_type != nullptr && _current_tid->GetType(id) == "UNDEFINED") {
        find_type = find_type->GetParent();
    }
    if (find_type == nullptr) return "UNDEFINED";
    return find_type->GetType(id);
}

// add func types
// add operators

FunctionDef* TID::AddFunction(std::string id) {
    if (_functions.count(id) != 0) throw std::logic_error("There are two functions"
                                                          " with the same names");
    return _functions[id] = new FunctionDef;
}

StructDef* TID::AddStruct(std::string id) {
    if (_structs.count(id) != 0) throw std::logic_error("There are two structures"
                                                        " with the same name");
    return _structs[id] = new StructDef;
}


