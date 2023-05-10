//
// Created by А ну да on 07.05.2023.
//

#include "Executive.h"

int IsBasic(std::string type_name) {
    if (type_name == "int32") return 1;
    if (type_name == "int64") return 1;
    if (type_name == "float32") return 2;
    if (type_name == "float64") return 2;
    if (type_name == "bool") return 1;
    if (type_name == "char") return 1;
    if (type_name == "string") return 3;
    return 4;
}

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

UserType UserType::operator*(UserType other) {
    other._bool *= this->_bool;
    other._int32 *= this->_int32;
    other._int64 *= this->_int64;
    other._float32 *= this->_float32;
    other._float64 *= this->_float64;
    other._char *= this->_char;
    return other;
}

UserType UserType::operator/(UserType other) {
    try {
        other._bool /= this->_bool;
        other._int32 /= this->_int32;
        other._int64 /= this->_int64;
        other._float32 /= this->_float32;
        other._float64 /= this->_float64;
        other._char /= this->_char;
    }
    catch(...) {
        throw std::logic_error("division by 0");
    }
    return other;
}

UserType UserType::operator!() {
    UserType other;
    other._bool = !this->_bool;
    other._int32 = this->_int32 == 0 ? 1 : 0;
    other._int64 = this->_int64 == 0 ? 1 : 0;
    other._float32 = this->_float32 == 0 ? 1 : 0;
    other._float64 = this->_float64 == 0 ? 1 : 0;
    other._char = this->_char == 0 ? 1 : 0;
    return other;
}

UserType UserType::operator==(UserType other) {
    other._bool = this->_bool == other._bool;
    other._int32 = this->_int32 == other._int32;
    other._int64 = this->_int64 == other._int64;
    other._float32 = this->_float32 == other._float32;
    other._float64 = this->_float64 == other._float64;
    other._char = this->_char == other._char;
    return other;
}

UserType UserType::operator!=(UserType other) {
    return *this == !other;
}

UserType UserType::operator<(UserType other) {
    other._bool = this->_bool < other._bool;
    other._int32 = this->_int32 < other._int32;
    other._int64 = this->_int64 < other._int64;
    other._float32 = this->_float32 < other._float32;
    other._float64 = this->_float64 < other._float64;
    other._char = this->_char < other._char;
    return other;
}

UserType UserType::operator||(UserType other) {
    other._bool = this->_bool || other._bool;
    other._int32 = this->_int32 || other._int32;
    other._int64 = this->_int64 || other._int64;
    other._float32 = this->_float32 || other._float32;
    other._float64 = this->_float64 || other._float64;
    other._char = this->_char || other._char;
    return other;
}

UserType UserType::operator&&(UserType other) {
    return !(!*this || !other);
}

UserType UserType::operator<=(UserType other) {
    return *this < other || *this == other;
}

UserType UserType::operator>(UserType other) {
    return !(*this <= other);
}

UserType UserType::operator>=(UserType other) {
    return *this > other || *this == other;
}

UserType UserType::operator++() {
    this->_bool += 1;
    this->_int32 += 1;
    this->_int64 += 1;
    this->_float32 += 1;
    this->_float64 += 1;
    this->_char += 1;
    return *this;
}

UserType UserType::operator--() {
    this->_bool -= 1;
    this->_int32 -= 1;
    this->_int64 -= 1;
    this->_float32 -= 1;
    this->_float64 -= 1;
    this->_char -= 1;
    return *this;
}

UserType UserType::operator>>(UserType other) {
    long long n = other._int64;
    other = *this;
    for (long long i = 0; i < n; ++i) {
        other = *this * other;
    }
    return other;
}

UserType* UserType::operator[](UserType ind) {
    return _elements[ind._int32];
}

void UserType::SetEverythingToType() {
    if (IsBasic(this->_type_name) == 1) {
        this->_float32 = this->_int64;
        this->_float64 = this->_int64;
        return;
    }
    if (IsBasic(this->_type_name) == 2) {
        this->_bool = this->_float64;
        this->_int32 = this->_float64;
        this->_int64 = this->_float64;
        this->_char = this->_float64;
        return;
    }
}

void Executive::ClearResults() {
    while (_results.top().size() > 0) {
        _results.top().pop();
    }
}

void Executive::ExecuteProgram() {
    while (_pos < _cells.size()) {
        FindExec();
        while (_pos < _cells.size()) {
            switch(_cells[_pos]->type) {
                case(ADD) : {
                    AddVariable();
                    ++_pos;
                    continue;
                }
                case(GET) : {
                    _results.top().push(*_variables.top()[dynamic_cast<PolizGet*>(_cells[_pos])->name]);
                    ++_pos;
                    continue;
                }
                case(WRITE) : {
                    _write_memory = _variables.top()[dynamic_cast<PolizGet*>(_cells[_pos])->name];
                    ++_pos;
                    continue;
                }
                case(JUMP) : {
                    Jump();
                    continue;
                }
                case(FALSEJUMP) : {
                    FalseJump();
                    continue;
                }
                case(RETURN) : {
                    continue;
                }
                case(SYMBOL) : {
                    Symbol();
                    ++_pos;
                    continue;
                }
                case(FUNCJUMP) : {
                    continue;
                }
                case(METHODJUMP) : {
                    continue;
                }
                case(INPUT) : {
                    continue;
                }
                case(OUTPUT) : {
                    continue;
                }
                case(OPERATOR) : {
                    continue;
                }
                case(BRACKET) : {
                    continue;
                }

            }
        }
    }

}

void Executive::Jump() {
    _callStack.push(_pos);
    _pos = dynamic_cast<PolizJump*>(_cells[_pos])->pos;
}

void Executive::FalseJump() {
    UserType result = _results.top().top();
    _results.top().pop();
    result.SetEverythingToType();
    if (!result._bool) {
        _pos = dynamic_cast<PolizFalseJump*>(_cells[_pos])->pos;
    }
}

void Executive::FuncJump() {

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
        std::map<std::string, UserType*> members;
        GetMembers(type, members);
        variable = new UserType(type, name, members);
    }

    _write_memory = variable;

    _variables.top()[name] = variable;
}

void Executive::Symbol() {
    std::string symbol = dynamic_cast<PolizSymbol*>(_cells[_pos])->string;
    if (symbol == ";") {
        ClearResults();
        return;
    }

    UserType p2 = _results.top().top();
    _results.top().pop();
    if (symbol == "*-") {
        _results.top().push(-p2);
        return;
    }
    if (symbol == "--") {
        //криво криво кривокосоосо очень криво и косо
        _results.top().push(--p2);
        return;
    }
    if (symbol == "++") {
        //криво криво кривокосоосо очень криво и косо
        _results.top().push(++p2);
        return;
    }
    if (symbol == "!") {
        _results.top().push(!p2);
        return;
    }

    UserType p1 = _results.top().top();
    _results.pop();
    if (symbol == "+") {
        _results.top().push(p1 + p2);
    }
    if (symbol == "-") {
        _results.top().push(p1 - p2);
    }
    if (symbol == "*") {
        _results.top().push(p1 * p2);
    }
    if (symbol == "**") {
        _results.top().push(p1 >> p2);
    }
    if (symbol == "/") {
        _results.top().push(p1 / p2);
    }
    if (symbol == "==") {
        _results.top().push(p1 == p2);
    }
    if (symbol == "!=") {
        _results.top().push(p1 != p2);
    }
    if (symbol == "<") {
        _results.top().push(p1 < p2);
    }
    if (symbol == ">") {
        _results.top().push(p1 > p2);
    }
    if (symbol == "||") {
        _results.top().push(p1 || p2);
    }
    if (symbol == "&&") {
        _results.top().push(p1 && p2);
    }
    if (symbol == ">=") {
        _results.top().push(p1 >= p2);
    }
    if (symbol == "<=") {
        _results.top().push(p1 <= p2);
    }
}

void Executive::OpenDerivativeScope() {
    _variables.push(_variables.top());
}

void Executive::OpenCleanScope(std::vector<UserType> parameters, std::map<std::string, UserType*> members) {
    std::map<std::string, UserType*> new_scope = _globals;
    for (auto parameter : parameters) {
        new_scope[parameter._var_name] = new UserType();
        *new_scope[parameter._var_name] = parameter;
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
