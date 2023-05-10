//
// Created by А ну да on 07.05.2023.
//

#ifndef LANGUAGE_EXECUTIVE_H
#define LANGUAGE_EXECUTIVE_H

#include "PolizGenerator.h"
#include <iostream>
#include <string>
#include <fstream>

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

    UserType(std::string type, std::string name, std::map<std::string, UserType*> members) : _type_name(type),
                                                                                             _var_name(name) {
        _members = members;
        _is_basic = false;
    }

    UserType(std::string type, std::string name, bool is_array, int num_of_elems) : _type_name(type), _var_name(name) {
        _is_array = true;
        _elements.resize(num_of_elems);
        for (int i = 0; i < num_of_elems; ++i) {
            _elements[i] = new UserType;
        }
    }

    UserType(bool literal, int val) {
        _bool = val;
        _int32 = val;
        _int64 = val;
        _float64 = val;
        _float32 = val;
        _char = val;
        _type_name = "int64";
        _is_basic = true;
        _is_var = false;
    }

    UserType(bool literal, float val) {
        _bool = val;
        _int32 = val;
        _int64 = val;
        _float64 = val;
        _float32 = val;
        _char = val;
        _type_name = "float64";
        _is_basic = true;
        _is_var = false;
    }

    UserType(bool literal, std::string val) {
        _is_var = false;
        if (val.size() == 1) {
            _bool = val[0];
            _int32 = val[0];
            _int64 = val[0];
            _float64 = val[0];
            _float32 = val[0];
            _char = val[0];
            _type_name = "char";
        }
        else {
            _type_name = "string";
        }
        _string = val;
        _is_basic = true;
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
    UserType* operator[](UserType ind);
    UserType operator[](int ind);

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
    std::map<std::string, UserType*> _members;

    std::vector<UserType*> _elements;

    bool _is_basic;
    bool _is_array = false;
    bool _is_var = true;
};

class Executive {
public:
    void ReadPoliz(std::ifstream f);

    void ExecuteProgram();
private:
    std::vector<PolizCell*> _cells;
    std::stack<int> _callStack;
    int _pos;
    UserType* _write_memory;

    std::map<std::string, std::vector<std::string>> _member_list;
    std::map<std::string, std::vector<std::string>> _method_list;
    std::map<std::string, std::vector<std::string>> _parameter_list;

    std::stack<std::map<std::string, UserType*>> _variables;
    std::map<std::string, UserType*> _globals;
    std::stack<std::stack<UserType*>> _results;

    void ClearResults();
    void GetMembers(std::string type, std::map<std::string, UserType*>& members);

    void FindExec();
    void Expression();

    void Jump();
    void FalseJump();
    void FuncJump();
    void MethodJump();
    void OperatorJump();
    void Operator();
    void Symbol();

    void AddVariable();

    void Return();

    void OpenDerivativeScope();
    void OpenCleanScope(std::vector<UserType> parameters);
    void OpenCleanScope(std::vector<UserType> parameters, std::map<std::string, UserType*> members);
    void CloseScope();
    void ClearElement(UserType* member);

    void Input(UserType *member, std::string s);
    void Output(UserType *member, std::string s);

};


#endif //LANGUAGE_EXECUTIVE_H
