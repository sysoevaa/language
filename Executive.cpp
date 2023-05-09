//
// Created by А ну да on 07.05.2023.
//

#include "Executive.h"

UserType UserType::operator+(UserType other) {
    other._bool += this->_bool;
    other._int32 += this->_int32;
    other._int64 += this->_int64;
    other._float32 += this->_float32;
    other._float64 += this->_float64;
    other._char += this->_char;
    other._string += this->_string;
    return other;
}

UserType UserType::operator-() {
    UserType other = *this;
    other._bool = -this->_bool;
    other._int32 = -this->_int32;
    other._int64 = -this->_int64;
    other._float32 = -this->_float32;
    other._float64 = -this->_float64;
    other._char = -this->_char;
    return other;
}

UserType UserType::operator-(UserType other) {
    other = -other;
    return *this + other;
}



void Executive::ExecuteProgram() {

}

void Executive::Jump() {
    _callStack.push(_pos);
    _pos = dynamic_cast<PolizJump*>(_cells[_pos])->pos;
}

void Executive::AddVariable() {
    std::string name = dynamic_cast<PolizAdd*>(_cells[_pos])->name;
    std::string type = dynamic_cast<PolizAdd*>(_cells[_pos])->type;

    UserType* variable;

    if (IsBasic(type) == 1) {
        long long val = 0;
        variable = new UserType(type, name, val);
    }
    if (IsBasic(type) == 2) {
        long double val = 0;
        variable = new UserType(type, name, val);
    }
    if (IsBasic(type) == 3) {
        std::string val;
        variable = new UserType(type, name, val);
    }
    if (IsBasic(type) == 4) {
        std::vector<UserType*> members;
        variable = new UserType(type, name, members);
    }

    _results.push(variable);

    _variables.top()[name] = variable;
}

void Executive::OpenDerivativeScope() {
    _variables.push(_variables.top());
}

void Executive::OpenCleanScope(std::vector<UserType*> parameters) {
    std::map<std::string, UserType*> new_scope;
    for (auto parameter : parameters) {
        new_scope[parameter->_var_name] = parameter;
    }
}

void Executive::CloseScope() {
    std::map<std::string, UserType*>::iterator it;
    for (it = _variables.top().begin(); it != _variables.top().end(); ++it) {
        ClearElement(it->second);
    }
    _variables.pop();
}

void Executive::ClearElement(UserType *member) {
    if (member->_is_basic) {
        delete member;
        return;
    }
    for (auto it : member->_members) {
        ClearElement(it);
    }
}
