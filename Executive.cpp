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

UserType UserType::operator[](int ind) {
    return *_elements[ind];
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
                case(SCOPE) : {
                    OpenDerivativeScope();
                    ++_pos;
                    continue;
                }
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
                    Return();
                    continue;
                }
                case(SYMBOL) : {
                    Symbol();
                    ++_pos;
                    continue;
                }
                case(FUNCJUMP) : {
                    FuncJump();
                    continue;
                }
                case(METHODJUMP) : {
                    MethodJump();
                    continue;
                }
                case(INPUT) : {
                    Input(_write_memory, "");
                    continue;
                }
                case(OUTPUT) : {
                    while (!_results.top().empty()) {
                        Output(&_results.top().top(), "");
                    }
                    continue;
                }
                case(OPERATOR) : {
                    Operator();
                    ++_pos;
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
    std::string type = dynamic_cast<PolizFuncJump*>(_cells[_pos])->type;
    int count = dynamic_cast<PolizFuncJump*>(_cells[_pos])->count;
    int pos = dynamic_cast<PolizJump*>(_cells[_pos])->pos;
    std::vector<UserType> parameters;
    for (int i = 0; i < count; ++i) {
        parameters.push_back(_results.top().top());
        //rename
    }
    OpenCleanScope(parameters);
    _callStack.push(_pos);
    _pos = pos;
}

void Executive::MethodJump() {
    std::string type = dynamic_cast<PolizMethodJump*>(_cells[_pos])->type;
    std::string name = dynamic_cast<PolizMethodJump*>(_cells[_pos])->name;
    int count = dynamic_cast<PolizMethodJump*>(_cells[_pos])->count;
    int pos = dynamic_cast<PolizMethodJump*>(_cells[_pos])->pos;
    std::vector<UserType> parameters;
    std::vector<std::string> parameter_names = _parameter_list[name];
    for (int i = 0; i < count; ++i) {
        parameters.push_back(_results.top().top());
        parameters[i]._var_name = parameter_names[parameter_names.size() - 1 - i];
    }
    OpenCleanScope(parameters, _results.top().top()._members);
    _callStack.push(_pos);
    _pos = pos;
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

void Executive::Operator() {
    std::string symbol = dynamic_cast<PolizSymbol*>(_cells[_pos])->string;
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

    if (symbol == "=") {
        *_write_memory = p2;
        return;
    }


    UserType p1 = _results.top().top();
    _results.top().pop();

    if (symbol == ".") {
        _results.top().push(*p2._members[p1._var_name]);
        return;
    }
    if (symbol == "[]") {
        _results.top().push(*p2._elements[p1._int32]);
        return;
    }

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

void Executive::Symbol() {
    std::string symbol = dynamic_cast<PolizSymbol*>(_cells[_pos])->string;
    if (symbol == ";") {
        ClearResults();
        return;
    }


}

void Executive::OpenDerivativeScope() {
    _variables.push(_variables.top());
}

void Executive::OpenCleanScope(std::vector<UserType> parameters) {
    std::map<std::string, UserType*> new_scope = _globals;
    for (auto parameter : parameters) {
        new_scope[parameter._var_name] = new UserType();
        *new_scope[parameter._var_name] = parameter;
    }
    _variables.push(new_scope);
    _results.push(std::stack<UserType>());
}

void Executive::OpenCleanScope(std::vector<UserType> parameters, std::map<std::string, UserType*> members) {
    std::map<std::string, UserType*> new_scope = _globals;
    for (auto parameter : parameters) {
        new_scope[parameter._var_name] = new UserType();
        *new_scope[parameter._var_name] = parameter;
    }
    new_scope.insert(members.begin(), members.end());
    _variables.push(new_scope);
    _results.push(std::stack<UserType>());
}

void Executive::CloseScope() {
    std::map<std::string, UserType*>::iterator it;
    for (it = _variables.top().begin(); it != _variables.top().end(); ++it) {
        ClearElement(it->second);
    }
    _variables.pop();
    _results.pop();
}

void Executive::Return() {
    UserType result = _results.top().top();
    CloseScope();
    _results.top().push(result);
    _pos = _callStack.top() + 1;
    _callStack.pop();
}

void Executive::ClearElement(UserType *member) {
    if (member->_is_basic) {
        delete member;
        return;
    }
    for (auto it : member->_members) {
        ClearElement(it.second);
    }
}

void Executive::Input(UserType *member, std::string s = "") {
    if (member->_is_array) {
        for (int i = 0; i < member->_elements.size(); ++i) {
            std::cout << s << "type in element no. " << i << " : ";
            if (IsBasic(member[i]._type_name) == 1) {
                long long input;
                std::cin >> input;
                member[i] = UserType(member->_type_name, member->_var_name, input);
                continue;
            }
            if (IsBasic(member[i]._type_name) == 2) {
                long double input;
                std::cin >> input;
                member[i] = UserType(member->_type_name, member->_var_name, input);
                continue;
            }
            if (IsBasic(member[i]._type_name) == 3) {
                std::string input;
                std::cin >> input;
                member[i] = UserType(member->_type_name, member->_var_name, input);
                continue;
            }
            for (auto it : member->_members) {
                std::cout << "type in " << it.first << " : ";
                Input(it.second, s + " ");
            }
        }
        return;
    }
    if (IsBasic(member->_type_name) == 1) {
        long long input;
        std::cin >> input;
        *member = UserType(member->_type_name, member->_var_name, input);
        return;
    }
    if (IsBasic(member->_type_name) == 2) {
        long double input;
        std::cin >> input;
        *member = UserType(member->_type_name, member->_var_name, input);
        return;
    }
    if (IsBasic(member->_type_name) == 3) {
        std::string input;
        std::cin >> input;
        *member = UserType(member->_type_name, member->_var_name, input);
        return;
    }
    for (auto it : member->_members) {
        std::cout << s << "type in " << it.first << " : ";
        Input(it.second, s + " ");
    }
}

void Executive::Output(UserType* member, std::string s = "") {
    if (member->_is_array) {
        for (int i = 0; i < member->_elements.size(); ++i) {
            std::cout << s << "element no. " << i << " : ";
            if (IsBasic(member[i]._type_name) == 1) {
                long long output = member[i]._int64;
                std::cout << output;
                continue;
            }
            if (IsBasic(member[i]._type_name) == 2) {
                long double output = member[i]._float64;
                std::cout << output;
                continue;
            }
            if (IsBasic(member[i]._type_name) == 3) {
                std::string output = member[i]._string;
                std::cout << output;
                continue;
            }
            for (auto it : member->_members) {
                std::cout << s << it.first << " : ";
                Output(it.second, s + " ");
            }
        }
        return;
    }
    if (IsBasic(member->_type_name) == 1) {
        long long output = member->_int64;
        std::cout << output;
        return;
    }
    if (IsBasic(member->_type_name) == 2) {
        long double output = member->_float64;
        std::cout << output;
        return;
    }
    if (IsBasic(member->_type_name) == 3) {
        std::string output = member->_string;
        std::cout << output;
        return;
    }
    for (auto it : member->_members) {
        std::cout << s << "type in " << it.first << " : ";
        Output(it.second, s + " ");
    }
}


