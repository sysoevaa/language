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
    other._is_var = false;
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
    other._is_var = false;
    return other;
}

UserType UserType::operator-(UserType other) {
    other = -other;
    other._is_var = false;
    return *this + other;
}

UserType UserType::operator*(UserType other) {
    other._bool *= this->_bool;
    other._int32 *= this->_int32;
    other._int64 *= this->_int64;
    other._float32 *= this->_float32;
    other._float64 *= this->_float64;
    other._char *= this->_char;
    other._is_var = false;
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
    other._is_var = false;
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
    other._is_var = false;
    return other;
}

UserType UserType::operator==(UserType other) {
    other._bool = this->_bool == other._bool;
    other._int32 = this->_int32 == other._int32;
    other._int64 = this->_int64 == other._int64;
    other._float32 = this->_float32 == other._float32;
    other._float64 = this->_float64 == other._float64;
    other._char = this->_char == other._char;
    other._is_var = false;
    return other;
}

UserType UserType::operator!=(UserType other) {
    other._is_var = false;
    return *this == !other;
}

UserType UserType::operator<(UserType other) {
    other._bool = this->_bool < other._bool;
    other._int32 = this->_int32 < other._int32;
    other._int64 = this->_int64 < other._int64;
    other._float32 = this->_float32 < other._float32;
    other._float64 = this->_float64 < other._float64;
    other._char = this->_char < other._char;
    other._is_var = false;
    return other;
}

UserType UserType::operator||(UserType other) {
    other._bool = this->_bool || other._bool;
    other._int32 = this->_int32 || other._int32;
    other._int64 = this->_int64 || other._int64;
    other._float32 = this->_float32 || other._float32;
    other._float64 = this->_float64 || other._float64;
    other._char = this->_char || other._char;
    other._is_var = false;
    return other;
}

UserType UserType::operator&&(UserType other) {
    other = !(!*this || !other);
    other._is_var = false;
    return other;
}

UserType UserType::operator<=(UserType other) {
    other = *this < other || *this == other;
    other._is_var = false;
    return other;
}

UserType UserType::operator>(UserType other) {
    other = !(*this <= other);
    other._is_var = false;
    return other;
}

UserType UserType::operator>=(UserType other) {
    other = *this > other || *this == other;
    other._is_var = false;
    return other;
}

UserType UserType::operator++() {
    this->_bool += 1;
    this->_int32 += 1;
    this->_int64 += 1;
    this->_float32 += 1;
    this->_float64 += 1;
    this->_char += 1;
    UserType other = *this;
    other._is_var = false;
    return other;
}

UserType UserType::operator--() {
    this->_bool -= 1;
    this->_int32 -= 1;
    this->_int64 -= 1;
    this->_float32 -= 1;
    this->_float64 -= 1;
    this->_char -= 1;
    UserType other = *this;
    other._is_var = false;
    return other;
}

UserType UserType::operator>>(UserType other) {
    long long n = other._int64;
    other = *this;
    for (long long i = 0; i < n; ++i) {
        other = *this * other;
    }
    other._is_var = false;
    return other;
}

UserType* UserType::operator[](UserType ind) {
    return _elements[ind._int32];
}

UserType UserType::operator[](int ind) {
    return *_elements[ind];
}

UserType UserType::operator<<(UserType other) {
    std::cout << "\n\ncompleting = \n";
    std::string type = this->_type_name;
    std::string name = this->_var_name;
    std::cout << this->_var_name << " " << this->_int64 << " = " << other._var_name << " " << other._int64 << "\n";

    *this = other;

    std::cout << this->_var_name << " " << this->_int64 << " = " << other._var_name << " " << other._int64 << "\n";

    this->_is_var = true;
    this->_var_name = name;

    std::cout << this->_var_name << " " << this->_int64 << " = " << other._var_name << " " << other._int64 << "\n";
    std::cout << '\n';
    return *this;
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

void Executive::ReadPoliz(std::ifstream& f) {
    int n;
    f >> n;
    _execs.resize(n);
    for (int i = 0; i < n; ++i) {
        f >> _execs[i];
    }

    f >> n;
    std::string name, type;
    for (int i = 0; i < n; ++i) {
        f >> type;
        f >> name;
        UserType* global;
        if (IsBasic(type) == 1) {
            long long val = 0;
            global = new UserType(type, name, val);
        }
        if (IsBasic(type) == 2) {
            long double val = 0;
            global = new UserType(type, name, val);
        }
        if (IsBasic(type) == 3) {
            std::string val = "";
            global = new UserType(type, name, val);
        }
        if (IsBasic(type) == 4) {
            std::map<std::string, UserType*> members;
            global = new UserType(type, name, members);
            _to_fill_members.push_back(global);
        }
        _globals[name] = global;
    }
    int count;
    std::vector<std::string> parameters;
    std::string type_name;

    f >> n;
    for (int i = 0; i < n; ++i) {
        f >> name;
        f >> type;
        f >> count;
        parameters.resize(count);
        for (int j = 0; j < count; ++j) {
            f >> type;
            f >> parameters[j];
        }
        _parameter_list[name] = parameters;
    }

    f >> n;
    for (int i = 0; i < n; ++i) {
        f >> type_name;
        f >> type;
        f >> name;
        _member_list[type_name].push_back({type, name});
    }

    f >> n;
    for (int i = 0; i < n; ++i) {
        f >> type;
        f >> name;
        f >> count;
        parameters.resize(count);
        for (int j = 0; j < count; ++j) {
            f >> parameters[j];
        }
        _parameter_list[type + '&' + name] = parameters;
    }

    //считать все структуры и т/п/
    std::string s;
    while (f >> s) {
        if (s == "add") {
            std::string name;
            f >> name;
            std::string type = " ";
            f >> type;
            _cells.push_back(new PolizAdd(type, name));
            continue;
        }
        if (s == "get") {
            std::string name;
            f >> name;
            type = "";
            _cells.push_back(new PolizGet(name, type));
            continue;
        }
        if (s == "write") {
            std::string name;
            f >> name;
            std::string type;
            f >> type;
            _cells.push_back(new PolizAdd(name, type));
            continue;
        }
        if (s == "jump") {
            int pos;
            f >> pos;
            _cells.push_back(new PolizJump(pos));
            continue;
        }
        if (s == "falsejump") {
            int pos;
            f >> pos;
            _cells.push_back(new PolizFalseJump(pos));
            continue;
        }
        if (s == "return") {
            int pos;
            //f >> pos;
            _cells.push_back(new PolizReturn);
            continue;
        }
        if (s == "symbol") {
            std::string val;
            f >> val;
            _cells.push_back(new PolizSymbol(val));
            continue;
        }
        if (s == "funcjump") {
            int pos;
            f >> pos;
            std::string type = "";
            int count = 0;
            std::string name = "";
            f >> name;
            _cells.push_back(new PolizFuncJump(pos, count, type, name));
            continue;
        }
        if (s == "methodjump") {
            int pos;
            f >> pos;
            std::string type;
            //f >> type;
            std::string name;
            f >> name;


            int count;
            //f >> count;
            _cells.push_back(new PolizMethodJump(pos, count, type, name));
            continue;
        }
        if (s == "output") {
            int count;
            f >> count;
            _cells.push_back(new PolizOutput(count));
            continue;
        }
        if (s == "input") {
            int count;
            f >> count;
            _cells.push_back(new PolizInput(count));
            continue;
        }
        if (s == "operator") {
            std::string oper;
            f >> oper;
            if (oper == "~") {
                oper = "*-";
            }
            int prior = 0;
            _cells.push_back(new PolizOperator(oper, prior));
        }
        if (s == "bracket") {
            _cells.push_back(new PolizBracket(0));
        }
    }

    for (auto a : _to_fill_members) {
        GetMembers(a->_type_name, a->_members);
    }
}

void Executive::ClearResults() {
    while (_results.top().size() > 0) {
        _results.top().pop();
    }
}

void Executive::ResultOut() {
    std::stack<UserType*> result = _results.top();
    std::cout << "pos : " << _pos << "\n";
    std::cout << "results : ";
    while (!result.empty()) {
        std::cout << result.top()->_var_name << " " << result.top()->_int64 << " , ";
        result.pop();
    }
    std::cout << "\n";
    std::cout << "variables: ";
    for (auto a : _variables.top()) {
        std::cout << a.first << " " << a.second->_var_name << " " << a.second->_int64 << " , ";
    }
    std::cout << "\n";
}

void Executive::ExecuteProgram() {
    while (_pos < _cells.size() && !_time_to_end) {
        FindExec();
        while (_pos < _cells.size() && !_time_to_end && _variables.size() > 0) {
            std::cout << "\n";
            std::stack<UserType*> p = _results.top();
            int pos = _pos;
            ResultOut();
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
                    _results.top().push(_variables.top()[dynamic_cast<PolizGet*>(_cells[_pos])->name]);
                    ++_pos;
                    continue;
                }
                case(WRITE) : {
                    _results.top().push(_variables.top()[dynamic_cast<PolizGet*>(_cells[_pos])->name]);
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
                        Output(_results.top().top(), "");
                        _results.top().pop();
                    }
                    continue;
                }
                case(OPERATOR) : {
                    Operator();
                    std::cout << "\n";
                    ++_pos;
                    continue;
                }
                case(BRACKET) : {
                    ++_pos;
                    continue;
                }
                default:
                    break;

            }
        }
    }

}

void Executive::FindExec() {
    if (_exec_ind >= _execs.size()) {
        _time_to_end = true;
        return;
    }
    _pos = _execs[_exec_ind];
    ++_exec_ind;
    _variables.push(_globals);
    _results.push(std::stack<UserType*>());
}

void Executive::Jump() {
    _callStack.push(_pos);
    _pos = dynamic_cast<PolizJump*>(_cells[_pos])->pos;
}

void Executive::FalseJump() {
    UserType* result = _results.top().top();
    _results.top().pop();
    result->SetEverythingToType();
    if (!result->_bool) {
        _pos = dynamic_cast<PolizFalseJump*>(_cells[_pos])->pos;
    }
    delete result;
}

void Executive::FuncJump() {
    std::string type = dynamic_cast<PolizFuncJump*>(_cells[_pos])->type;
    std::string name = dynamic_cast<PolizFuncJump*>(_cells[_pos])->name;
    //int count = dynamic_cast<PolizFuncJump*>(_cells[_pos])->count;
    int count = _parameter_list[name].size();
    int pos = dynamic_cast<PolizFuncJump*>(_cells[_pos])->pos;
    std::vector<UserType> parameters;
    std::vector<std::string> parameter_names = _parameter_list[name];
    std::vector<UserType*> to_delete;
    std::stack<UserType*> k = _results.top();
    for (int i = 0; i < count; ++i) {
        parameters.push_back(*_results.top().top());
        if (!_results.top().top()->_is_var) {
            to_delete.push_back(_results.top().top());
        }
        _results.top().pop();
        parameters[i]._var_name = parameter_names[parameter_names.size() - 1 - i];
    }
    OpenCleanScope(parameters);
    for (auto a : to_delete) {
        delete a;
    }
    _callStack.push(_pos);
    _pos = pos;
}

void Executive::MethodJump() {
    //std::string type = dynamic_cast<PolizMethodJump*>(_cells[_pos])->type;
    std::string type = _results.top().top()->_type_name;
    _results.pop();
    std::string name = dynamic_cast<PolizMethodJump*>(_cells[_pos])->name;
    int count = dynamic_cast<PolizMethodJump*>(_cells[_pos])->count;
    int pos = dynamic_cast<PolizMethodJump*>(_cells[_pos])->pos;
    std::vector<UserType> parameters;
    std::vector<std::string> parameter_names = _parameter_list[type + '&' + name];
    std::vector<UserType*> to_delete;
    for (int i = 0; i < count; ++i) {
        parameters.push_back(*_results.top().top());
        if (!_results.top().top()->_is_var) {
            to_delete.push_back(_results.top().top());
        }
        _results.top().pop();
        parameters[i]._var_name = parameter_names[parameter_names.size() - 1 - i];
    }
    OpenCleanScope(parameters, _results.top().top()->_members);
    for (auto a : to_delete) {
        delete a;
    }
    _callStack.push(_pos);
    _pos = pos;
}

void Executive::AddVariable() {
    std::string name = dynamic_cast<PolizAdd*>(_cells[_pos])->name;
    std::string type = dynamic_cast<PolizAdd*>(_cells[_pos])->type;

    UserType* variable = nullptr;
    std::cout << "\n ADDING NEW VARIABLE: ";

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

    std::cout << variable->_var_name << " " << variable->_int64 << "\n";
    //_write_memory = variable;
    _locals.push_back(variable);
    _results.top().push(variable);

    _variables.top()[name] = variable;
}

void Executive::Operator() {
    std::string symbol = dynamic_cast<PolizOperator*>(_cells[_pos])->oper;
    std::cout << symbol << ": ";
    std::stack<UserType*> p = _results.top();

    UserType* p2 = _results.top().top();
    _results.top().pop();
    std::cout << p2->_var_name << " " << p2->_int64 << " ";

    UserType* result = new UserType;

    bool happened = false;

    if (symbol == "*-") {
        *result = -(*p2);
        _results.top().push(result);
        happened = true;
    }
    if (symbol == "--") {
        *result = --(*p2);
        _results.top().push(result);
        happened = true;
    }
    if (symbol == "++") {
        *result = ++(*p2);
        _results.top().push(result);
        happened = true;
    }
    if (symbol == "!") {
        *result = !(*p2);
        _results.top().push(result);
        happened = true;
    }

    if (happened) {
        if (!p2->_is_var) {
            delete p2;
        }
        _results.top().pop();
        return;
    }

    UserType* p1 = _results.top().top();
    _results.top().pop();
    std::cout << p1->_var_name << " " << p1->_int64 << " ";

    if (symbol == ".") {
        _results.top().push(p2->_members[p1->_var_name]);
        if (!p2->_is_var) {
            delete p2;
        }

        if (!p1->_is_var) {
            delete p1;
        }
        return;
    }
    if (symbol == "[]") {
        _results.top().push(p2->_elements[p1->_int32]);
        if (!p2->_is_var) {
            delete p2;
        }

        if (!p1->_is_var) {
            delete p1;
        }
        return;
    }
    if (symbol == "=") {
        *p1 << *p2;
        if (!p2->_is_var) {
            delete p1;
        }
        return;
    }

    if (symbol == "+") {
        *result = *p1 + *p2;
    }
    if (symbol == "-") {
        *result = *p1 - *p2;
    }
    if (symbol == "*") {
        *result = *p1 - *p2;
    }
    if (symbol == "**") {
        *result = *p1 >> *p2;
    }
    if (symbol == "/") {
        *result = *p1 / *p2;
    }
    if (symbol == "==") {
        *result = *p1 == *p2;
    }
    if (symbol == "!=") {
        *result = *p1 != *p2;
    }
    if (symbol == "<") {
        *result = *p1 < *p2;
    }
    if (symbol == ">") {
        *result = *p1 > *p2;
    }
    if (symbol == "||") {
        *result = *p1 || *p2;
    }
    if (symbol == "&&") {
        *result = *p1 && *p2;
    }
    if (symbol == ">=") {
        *result = *p1 >= *p2;
    }
    if (symbol == "<=") {
        *result = *p1 <= *p2;
    }
    std::cout << "result: " << result->_var_name << " " << result->_int64;
    _results.top().push(result);

    if (!p2->_is_var) {
        delete p2;
    }

    if (!p1->_is_var) {
        delete p1;
    }

}

void Executive::Symbol() {
    std::string symbol = dynamic_cast<PolizSymbol*>(_cells[_pos])->string;
    UserType* result = nullptr;
    if (symbol[0] <= '9' && symbol[0] >= '0') {
        bool floating = false;
        for (auto k : symbol) {
            if (k == '.') {
                floating = true;
            }
        }
        if (floating) {
            float val = std::stof(symbol);
            result = new UserType(false, val);
        }
        else {
            int val = std::stoi(symbol);
            result = new UserType(false, val);

        }
        _results.top().push(result);
        return;
    }
    result = new UserType(false, symbol);
    _results.top().push(result);

}

void Executive::OpenDerivativeScope() {
    _variables.push(_variables.top());
}

void Executive::OpenCleanScope(std::vector<UserType> parameters) {
    std::map<std::string, UserType*> new_scope = _globals;
    for (auto parameter : parameters) {
        new_scope[parameter._var_name] = new UserType();
        *new_scope[parameter._var_name] = parameter;
        new_scope[parameter._var_name]->_is_var = true;
    }
    _variables.push(new_scope);
    _results.push(std::stack<UserType*>());
}

void Executive::OpenCleanScope(std::vector<UserType> parameters, std::map<std::string, UserType*> members) {
    std::map<std::string, UserType*> new_scope = _globals;
    for (auto parameter : parameters) {
        new_scope[parameter._var_name] = new UserType();
        *new_scope[parameter._var_name] = parameter;
        new_scope[parameter._var_name]->_is_var = true;
    }
    new_scope.insert(members.begin(), members.end());
    _variables.push(new_scope);
    _results.push(std::stack<UserType*>());
}

void Executive::CloseScope() {
    std::map<std::string, UserType*>::iterator it;
    for (it = _variables.top().begin(); it != _variables.top().end(); ++it) {
        ClearElement(it->second);
    }
   /* for (auto a : _locals) {
        delete a;
    }*/
    _variables.pop();
    _results.pop();
}

void Executive::Return() {
    if (_callStack.empty()) {
        FindExec();
        return;
    }
    UserType* result = _results.top().top();
    CloseScope();
    _results.top().push(result);
    _pos = _callStack.top() + 2;
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

void Executive::GetMembers(std::string class_type, std::map<std::string, UserType *> &members) {
    std::vector<std::pair<std::string, std::string>> member_list = _member_list[class_type];
    for (auto elem : member_list) {
        std::string name = elem.second, type = elem.first;
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
            std::map<std::string, UserType*> members2;
            GetMembers(type, members);
            variable = new UserType(type, name, members2);
        }
        members[name] = variable;
    }
}


