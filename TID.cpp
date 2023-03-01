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

std::vector<std::string> FunctionDef::GetParameters() {
    return _formal_parameters;
}

FunctionDef::FunctionDef() {
    parent = nullptr;
}

FunctionDef* TID::AddFunction(std::string id) {
    if (_functions.count(id) != 0) throw std::logic_error("There are two functions"
                                                          " with the same names");
    return _functions[id] = new FunctionDef;
}

StructDef* TID::AddStruct(std::string id) {
    if (_structs.count(id) != 0) throw std::logic_error("There are two structures "
                                                        "with the same name");
    return _structs[id] = new StructDef;
}

