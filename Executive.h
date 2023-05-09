//
// Created by А ну да on 07.05.2023.
//

#ifndef LANGUAGE_EXECUTIVE_H
#define LANGUAGE_EXECUTIVE_H

#include "PolizGenerator.h"

int IsBasic(std::string type_name);

struct UserType {
    UserType() {
        _type_name = "&null";
        _var_name = "&null";
        _is_basic = false;
    }

    UserType(std::string type, std::string name, long long val_int) : _type_name(type) {
        _var_name = name;
        _bool = val_int;
        _int32 = val_int;
        _int64 = val_int;
        _float32 = val_int;
        _float64 = val_int;
        _char = val_int;
        _is_basic = true;
    }

    UserType(std::string type, std::string name, long double val_float) : _type_name(type) {
        _var_name = name;
        _bool = val_float;
        _int32 = val_float;
        _int64 = val_float;
        _float32 = val_float;
        _float64 = val_float;
        _char = val_float;
        _is_basic = true;
    }

    UserType(std::string type, std::string name, std::string string) : _type_name(type), _var_name(name) {
        _string = string;
        _is_basic = true;
    }

    UserType(std::string type, std::string name, std::vector<UserType*> members) : _type_name(type), _var_name(name) {
        _members = members;
        _is_basic = false;
    }

    UserType operator+(UserType other);
    UserType operator-();
    UserType operator-(UserType other);
    UserType operator*(UserType other);
    UserType operator/(UserType other);
    UserType operator!();
    UserType operator==(UserType other);
    UserType operator!=(UserType other);
    UserType operator<(UserType other);
    UserType operator&&(UserType other);
    UserType operator||(UserType other);
    UserType operator>(UserType other);
    UserType operator<=(UserType other);
    UserType operator>=(UserType other);
    UserType operator++();
    UserType operator--();
    UserType operator>>(UserType other); //замена степени

    void SetEverythingToType();


    std::string _type_name;
    std::string _var_name;

    bool _bool;
    int _int32;
    long long _int64;
    float _float32;
    long double _float64;
    char _char;

    std::string _string;
    std::vector<UserType*> _members;

    bool _is_basic;
};

class Executive {
public:
    void ReadPoliz(std::string& s);

    void ExecuteProgram();
private:
    std::vector<PolizCell*> _cells;
    std::stack<int> _callStack;
    int _pos;

    std::stack<std::map<std::string, UserType*>> _variables;
    std::map<std::string, UserType*> _globals;
    std::stack<UserType*> _results;

    void ClearResults();

    void FindExec();
    void Expression();

    void Jump();
    void FalseJump();
    void OperatorJump();
    void Symbol();

    void AddVariable();

    void OpenDerivativeScope();
    void OpenCleanScope(std::vector<UserType*> parameters);
    void CloseScope();
    void ClearElement(UserType* member);

};


#endif //LANGUAGE_EXECUTIVE_H
