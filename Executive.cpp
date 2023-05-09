//
// Created by А ну да on 07.05.2023.
//

#include "Executive.h"

void Executive::ExecuteProgram() {

}

void Executive::Jump() {
    _callStack.push(_pos);
    _pos = dynamic_cast<PolizJump*>(_cells[_pos])->pos;
}

void Executive::AddVariable() {

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
