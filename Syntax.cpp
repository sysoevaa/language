#include "Lexeme.h"
#include "Syntax.h"

void SyntaxAnalyser::gc() {
    ++_ind;
    if (_ind >= _lex.size()) throw std::logic_error("unexpected end of file");
}

bool SyntaxAnalyser::type() {
    if (_lex[_ind].type == "variable") {
        return true;
    }
    if (_lex[_ind].string == "void") {
        return true;
    }
    if (_lex[_ind].string == "char") {
        return true;
    }
    if (_lex[_ind].string == "int32") {
        return true;
    }
    if (_lex[_ind].string == "int64") {
        return true;
    }
    if (_lex[_ind].string == "float32") {
        return true;
    }
    if (_lex[_ind].string == "float64") {
        return true;
    }
    if (_lex[_ind].string == "string") {
        return true;
    }
    if (_lex[_ind].string == "bool") {
        return true;
    }
    return false;
}

void SyntaxAnalyser::program() {
    globalNamespaceNoExec();
    if (_lex[_ind].string != "exec") throw std::logic_error("\"exec\" expected");
    gc();
    functionDefinition();
    globalNamespace();
}

void SyntaxAnalyser::globalNamespaceNoExec() {
    if (_lex[_ind].string == "struct") {
        object();
        globalNamespaceNoExec();
        return;
    }
    if (_lex[_ind].string == "array") {
        array_def();
        globalNamespaceNoExec();
        return;
    }
    if (type()) {
        std::string var_type = _lex[_ind].string;
        if (!_tid->IsTypeExist(_lex[_ind].string)) throw std::logic_error("type does not exist");
        if (_ind + 2 >= _lex.size()) throw std::logic_error("unexpected end of file");
        if (_lex[_ind + 1].string == "cast") {
            type_cast_def();
            globalNamespaceNoExec();
            return;
        }
        else if (_lex[_ind + 1].type != "variable") {
            throw std::logic_error("expected variable");
        }
        if (_lex[_ind + 2].string == "(") {
            functionDefinition();
            globalNamespaceNoExec();
            return;
        }
        std::string var_name = _lex[_ind + 1].string;
        gc();
        gc();
        if (_lex[_ind].string == ";") {
            gc();
            _tid->AddVariable(var_type, var_name);
            globalNamespaceNoExec();
            return;
        }
        if (_lex[_ind].string == "=") {
            gc();
            expression();
            Lexeme lex = expCheck.GetType();
            expCheck.Clear();
            if (IsEqualTypes(var_type, lex.string) == "error") {
                throw std::logic_error("trying to put " + lex.string + " into " + var_type);
            }
            if (_lex[_ind].string == ";") gc();
            _tid->AddVariable(var_type, var_name);
            globalNamespaceNoExec();
            return;
        }
        throw std::logic_error("unexpected symbols");
    }
    return;
}

void SyntaxAnalyser::globalNamespace() {
    if (_lex[_ind].string == "struct") {
        object();
        globalNamespace();
        return;
    }
    if (_lex[_ind].string == "array") {
        array_def();
        globalNamespace();
        return;
    }
    if (_lex[_ind].string == "exec") {
        gc();
        functionDefinition();
        globalNamespace();
        return;
    }
    if (type()) {
        if (!_tid->IsTypeExist(_lex[_ind].string)) throw std::logic_error("type does not exist");
        if (_ind + 2 >= _lex.size()) throw std::logic_error("unexpected end of file");
        if (_lex[_ind + 1].string == "cast") {
            type_cast_def();
            globalNamespace();
            return;
        }
        else if (_lex[_ind + 1].type != "variable") throw std::logic_error("variable expected");
        if (_lex[_ind + 2].string == "(") {
            functionDefinition();
            globalNamespace();
            return;
        }
        gc();
        gc();
        if (_lex[_ind + 2].string == ";") {
            _tid->AddVariable(_lex[_ind].string, _lex[_ind + 1].string);
            gc();
            globalNamespace();
            return;
        }
        if (_lex[_ind + 2].string == "=") {
            _tid->AddVariable(_lex[_ind].string, _lex[_ind + 1].string);
            std::string type = _lex[_ind].string;
            gc();
            expression();
            Lexeme lex = expCheck.GetType();
            expCheck.Clear();
            if (lex.string != type) {
                throw std::logic_error("trying to put " + lex.string + " into the " + type);
            }
            globalNamespace();
            return;
        }
        throw std::logic_error("unexpected symbols");
    }
    return;
}

void SyntaxAnalyser::object() {
    //how to add type
    if (_lex[_ind].string != "struct") throw std::logic_error("\"struct\" expected");
    gc();
    if (_lex[_ind].type != "variable") throw std::logic_error("variable expected");
    _tid->AddStruct(_lex[_ind].string);
    object_name = _lex[_ind].string;
    gc();
    if (_lex[_ind].string != "{") throw std::logic_error("\"{\" expected");
    gc();
   // _tid->OpenScope();
    member();
    _tid->CloseScope();
    if (_lex[_ind].string != "}") throw std::logic_error("\"}\"expected");
    gc();
}

void SyntaxAnalyser::member() {
    if (_lex[_ind].string == "construct") {
        construct();
        std::string void_ = "void";
        //_tid->AddFunction(object_name, _parameter_def_arr, void_);
        member();
        return;
    }
    if (_lex[_ind].string == "array") {
        array_def();
        member();
        return;
    }
    if (type()) {
        if (!_tid->IsTypeExist(_lex[_ind].string)) throw std::logic_error("type does not exist");
        std::string member_type = _lex[_ind].string;
        if (_ind + 2 >= _lex.size()) throw std::logic_error("unexpected end of file");
        if (_lex[_ind + 1].string == "overload") {
            overload();
            member();
            return;
        }
        else if (_lex[_ind + 1].type != "variable") throw std::logic_error("variable expected");
        std::string member_name = _lex[_ind + 1].string;
        if (_lex[_ind + 2].string == "(") {
            is_method = true;
            method_return_type = member_type;
            method_name = member_name;
            functionDefinition();
            //_tid->AddFunction(member_name, _parameter_def_arr, member_type);

            _parameter_def_arr.clear();
            is_method = false;
            member();
            return;
        }
        gc();
        gc();
        if (_lex[_ind].string == ";") {
            gc();
            _tid->AddVariable(member_type, member_name);
            member();
            return;
        }
        if (_lex[_ind].string == "=") {
            gc();
            expression();
            Lexeme lex = expCheck.GetType();
            expCheck.Clear();
            if (lex.string != member_type) throw std::logic_error("unexpected type");
            _tid->AddVariable(member_type, member_name);
            member();
            return;
        }
        throw std::logic_error("unexpected symbols");
    }
    return;
}

void SyntaxAnalyser::construct() {
    is_in_function = true;
    if (_lex[_ind].string != "construct") throw std::logic_error("\"construct \" expected");
    gc();
    if (_lex[_ind].string != "(") throw std::logic_error("\"(\" expected");
    gc();
    parameterDef();
    _tid->AddConstructor(_parameter_def_arr);
    if (_lex[_ind].string != ")") throw std::logic_error("\")\" expected");
    gc();
    if (_lex[_ind].string != "{") throw std::logic_error("\"{\" expected");
    gc();

    //_tid->OpenScope();
    namepace();
    _tid->CloseScope();
    if (_lex[_ind].string != "}") throw std::logic_error("\"}\" expected");
    gc();
    is_in_function = false;
}

void SyntaxAnalyser::overload() {
    if (!type()) throw std::logic_error("type of function expected");

    if (!_tid->IsTypeExist(_lex[_ind].string)) throw std::logic_error("type does not exist");

    std::string ret_type = _lex[_ind].string;
    gc();

    if (_lex[_ind].string != "overload") throw std::logic_error("\"overload\" expected");
    gc();

    if (_lex[_ind].type != "binary") throw std::logic_error("binary operator expected");
    std::string operator_ = _lex[_ind].string;
    gc();

    if (_lex[_ind].string != "(") throw std::logic_error("\"( \" expected");
    gc();

    std::string type2;
    parameterDef();
    if (_parameter_def_arr.size() != 1) throw std::logic_error("incorrect number of parameters");
    type2 = _parameter_def_arr[0].first;

    if (_lex[_ind].string != ")") throw std::logic_error("\")\" expected");
    _tid->AddOverload(_parameter_def_arr[0].second, type2, operator_, ret_type);
    gc();

    if (_lex[_ind].string != "{") throw std::logic_error("\"{ \" expected");
    gc();

    _tid->OpenScope();
    namepace();
    _tid->CloseScope();

    if (_lex[_ind].string != "}") throw std::logic_error("\" } \" expected");
    gc();
}

void SyntaxAnalyser::expression() {
    if (_lex[_ind].type == "number") {
        std::string type_n = "float64";
        expCheck.Process(Lexeme("variable", type_n, 0));
        gc();
        if (_lex[_ind].type == "binary" || _lex[_ind].type == "power" || _lex[_ind].type == "bool") {
            expCheck.Process(_lex[_ind]);
            gc();
            expression();
        }
        return;
    }
    if (_lex[_ind].type == "string" || _lex[_ind].type == "char") {
        expCheck.Process(_lex[_ind]);
        gc();
        return;
    }
    if (_lex[_ind].type == "variable" || _lex[_ind].type == "number" || _lex[_ind].type == "char") {
        gc();
        if (_lex[_ind].string == "cast") {
            expCheck.Process(_lex[_ind - 1]);
            expCheck.Process(_lex[_ind]);
            --_ind;
            type_cast();
            expCheck.Process(Lexeme("variable", _tid->GetType(_lex[_ind - 1].string), 0));
            if (_lex[_ind].type == "binary" || _lex[_ind].type == "power" || _lex[_ind].type == "bool") {
                expCheck.Process(_lex[_ind]);
                gc();
                expression();
            }
            return;
        }

        if (_lex[_ind].string == "(") {
            std::string type_f = _tid->GetTypeFunction(_lex[_ind - 1].string);
            _parameter_arr = _tid->GetParameters(_lex[_ind - 1].string);
            expCheck.Process(Lexeme("variable", type_f, 0));
            parameters();
            if (_lex[_ind].string != ")") {
                throw std::logic_error("expected \")\"");
            }
            gc();
            if (_lex[_ind].type == "binary" || _lex[_ind].type == "power" || _lex[_ind].type == "bool") {
                expCheck.Process(_lex[_ind]);
                gc();
                expression();
            }
            return;
        }

        auto type1 = _tid->GetType(_lex[_ind - 1].string);
        expCheck.Process(Lexeme("variable", type1, 0));

        Lexeme exp_last = expCheck.GetLast();
        //std::cout << "exp_last = " << exp_last.string << "\n";
        //array check
        if (_lex[_ind].string == "[") {
            expCheck.Process(_lex[_ind]);
            gc();
            expression();
            if (_lex[_ind].string != "]") {
                throw std::logic_error ("] expected");
            }
            expCheck.Process(_lex[_ind]);
            gc();
            //std::cout << "exp_last = " << exp_last.string << "\n";
            expCheck.ChangeLast(GetArrayType(exp_last.string));
            exp_last = expCheck.GetLast();
        }

        while (_lex[_ind].string == ".") {
            gc();
            variable();
            if (_lex[_ind].string == "(") {
                _parameter_arr = _tid->GetMethodParameters(type1, _lex[_ind - 1].string);
                std::string method_name_ = _lex[_ind - 1].string;
                parameters();
                _parameter_arr.clear();
                type1 = _tid->GetMethodType(type1, method_name_);
                expCheck.ChangeLast(type1);
                if (_lex[_ind].string != ")") {
                    throw std::logic_error("expected \")\"");
                }
                gc();
                break;
            }
            type1 = _tid->GetMember(type1, _lex[_ind - 1].string);
            expCheck.ChangeLast(type1);
            exp_last = expCheck.GetLast();

            if (_lex[_ind].string == "[") {
                expCheck.Process(_lex[_ind]);
                gc();
                expression();
                if (_lex[_ind].string != "]") {
                    throw std::logic_error ("] expected");
                }
                expCheck.Process(_lex[_ind]);
                gc();
                expCheck.ChangeLast(GetArrayType(exp_last.string));
                exp_last = expCheck.GetLast();
            }
        }

        if (_lex[_ind].type == "binary" || _lex[_ind].type == "power" || _lex[_ind].type == "bool") {
            expCheck.Process(_lex[_ind]);
            gc();
            expression();
        }

        expCheck.TailMerge();
        return;
    }

    if (type()) {
        if (!_tid->IsTypeExist(_lex[_ind].string)) throw std::logic_error("type does not exist");
        expCheck.Process(_lex[_ind]);
        type_cast();
        expCheck.Process(_lex[_ind - 2]);
        expCheck.Process(Lexeme("variable", _tid->GetType(_lex[_ind - 1].string), 0));
        if (_lex[_ind].type == "binary" || _lex[_ind].type == "power" || _lex[_ind].type == "bool") {
            expCheck.Process(_lex[_ind]);
            gc();
            expression();
        }
        expCheck.TailMerge();
        return;
    }

    if (_lex[_ind].type == "bracket" && _lex[_ind].string == "(") {
        expCheck.Process(_lex[_ind]);
        gc();
        expression();
        if (_lex[_ind].string != ")") {
            throw std::logic_error("\")\" expected");
        }
        expCheck.Process(_lex[_ind]);
        gc();
        if (_lex[_ind].type == "binary" || _lex[_ind].type == "power" || _lex[_ind].type == "bool") {
            expCheck.Process(_lex[_ind]);
            gc();
            expression();
        }
        expCheck.TailMerge();
        return;
    }




    if (_lex[_ind].type == "unary") {
        expCheck.Process(_lex[_ind]);
        gc();

        if (_lex[_ind].type != "variable") throw std::logic_error("variable expected");

        auto type1 = _tid->GetType(_lex[_ind - 1].string);
        expCheck.Process(Lexeme("variable", type1, 0));

        Lexeme exp_last = expCheck.GetLast();

        //array check
        if (_lex[_ind].string == "[") {
            expCheck.Process(_lex[_ind]);
            gc();
            expression();
            if (_lex[_ind].string != "]") {
                throw std::logic_error ("] expected");
            }
            expCheck.Process(_lex[_ind]);
            gc();
            expCheck.ChangeLast(GetArrayType(exp_last.string));
            exp_last = expCheck.GetLast();
        }

        while (_lex[_ind].string == ".") {
            gc();
            variable();
            type1 = _tid->GetMember(type1, _lex[_ind - 1].string);
            expCheck.ChangeLast(type1);
            exp_last = expCheck.GetLast();
            if (_lex[_ind].string == "[") {
                expCheck.Process(_lex[_ind]);
                gc();
                expression();
                if (_lex[_ind].string != "]") {
                    throw std::logic_error ("] expected");
                }
                expCheck.Process(_lex[_ind]);
                gc();
                expCheck.ChangeLast(GetArrayType(exp_last.string));
                exp_last = expCheck.GetLast();
            }
        }

        if (_lex[_ind].type == "binary" || _lex[_ind].type == "power" || _lex[_ind].type == "bool") {
            expCheck.Process(_lex[_ind]);
            gc();
            expression();
        }

        expCheck.TailMerge();
        return;

    }
    throw std::logic_error("unexpected symbols");
}

void SyntaxAnalyser::parameters() {
    int i = 0;
    do {
        gc();
        if (_lex[_ind].string == ")") {
            if (i < _parameter_arr.size()) throw std::logic_error("not enough parameters");
            return;
        }
        std::vector<Lexeme> stack = expCheck.GetStack();
        expCheck.Clear();
        expression();
        Lexeme lex = expCheck.GetType();
        expCheck.Clear();
        expCheck.SetStack(stack);
        if (_parameter_arr.size() <= i) throw std::logic_error("Incorrect parameters in function call");
        if (IsEqualTypes(lex.string, _parameter_arr[i]) == "error") throw std::logic_error("parameter types do not match");
        ++i;
    } while (_lex[_ind].string == ",");
    if (i < _parameter_arr.size()) throw std::logic_error("not enough parameters");
}

void SyntaxAnalyser::parameterDef() {
    _parameter_def_arr.clear();
    std::pair<std::string, std::string> par;
    if (_lex[_ind].string == ")") return;
    do {
        if (_lex[_ind - 1].string != "(") gc();
        if (!type()) {
            throw std::logic_error("expected type name");
        }
        par.first = _lex[_ind].string;
        gc();
        if (_lex[_ind].type != "variable") {
            throw std::logic_error("variable expected");
        }
        par.second = _lex[_ind].string;
        gc();
        _parameter_def_arr.push_back(par);
    } while (_lex[_ind].string == ",");
}

void SyntaxAnalyser::determinantes() {
    if (_lex[_ind].string == "if") {
        gc();
        If();
    } else if (_lex[_ind].string == "while") {
        gc();
        While();
    } else if (_lex[_ind].string == "for") {
        gc();
        For();
    } else if (_lex[_ind].string == "do") {
        gc();
        if (_lex[_ind].string == "while") {
            gc();
            dowhile();
        }
        else {
            throw std::logic_error("\"while \" expected");
        }
    } else if (_lex[_ind].string == "print") {
        gc();
        print();
        if (_lex[_ind].string != ";") {
            throw std::logic_error("\";\" expected");
        }
        gc();
    } else if (_lex[_ind].string == "get") {
        //gc();
        get();
        if (_lex[_ind].string != ";") {
            throw std::logic_error("\";\" expected");
        }
        gc();
    } else if (_lex[_ind].string == "return") {
        if (!is_in_function) throw std::logic_error("return outside of function");
        gc();
        if (_lex[_ind].string != ";") {
            expression();
            Lexeme lex = expCheck.GetType();
            expCheck.Clear();
            auto return_type = _tid->GetCurrentReturnType();
            if (IsEqualTypes(return_type, lex.string) == "error") throw std::logic_error("trying to return " + lex.string +
           " in " + return_type + " function");
        }
        else {
            if (_tid->GetCurrentReturnType() != "void") {
                throw std::logic_error("retrun expression expected in non-void type function");
            }
        }
        if (_lex[_ind].string != ";") {
            throw std::logic_error("\";\" expected");
        }

        gc();
        IAMRETURNINGSOMETHING = true;
    } else {
        throw std::logic_error("unexpected symbols");
    }
}

void SyntaxAnalyser::lexpression() {
    if (_lex[_ind].string == "}") return;
    bool def = false;
    if (_lex[_ind].type == "unary") {
        //unary check
        gc();
        if (_lex[_ind].type != "variable") {
            throw std::logic_error("variable expected");
        }
        gc();
        return;
    }
    std::string name;
    std::string type1;
    if (type()) {
        if (_tid->IsFunctionExist(_lex[_ind].string)) {
            _parameter_arr = _tid->GetParameters(_lex[_ind].string);
            gc();
            parameters();
            if (_lex[_ind].string != ")") {
                throw std::logic_error("\")\" expected");
            }
            gc();
        } else {
            gc();
            if (_lex[_ind].type == "variable") {
                if (!_tid->IsTypeExist(_lex[_ind - 1].string)) throw std::logic_error("non existing type");
                _tid->AddVariable(_lex[_ind - 1].string, _lex[_ind].string);
                name = _lex[_ind].string;
                type1 = _lex[_ind - 1].string;
                def = true;
                gc();
            } else if (_tid->IsTypeExist(_lex[_ind - 1].string)) {
                throw std::logic_error("variable name expected");
            } else {
                name = _lex[_ind - 1].string;
                type1 = _tid->GetType(name);
            }
        }
        //} else if (_lex[_ind - 1].type == "keyword") {
    } else {
        throw std::logic_error("name expected");
    }
    //gc();
    if (_lex[_ind].string == "(") {
        if (def) {
            _parameter_arr = _tid->GetConstructorParameters(type1);
            parameters();
            if (_lex[_ind].string != ")") {
                throw std::logic_error("\")\" expected");
            }
            gc();
            return;
        }
        _tid->GetTypeFunction(_lex[_ind - 1].string);
        _parameter_arr = _tid->GetParameters(_lex[_ind - 1].string);
        parameters();
        if (_lex[_ind].string != ")") {
            throw std::logic_error("\")\" expected");
        }
        gc();
        return;
        //a
    }

    if (_lex[_ind].string == "[") {
        gc();
        expCheck.Clear();
        expression();
        Lexeme lex = expCheck.GetType();
        std::string int_type1 = "int32", int_type2 = "int64";
        if (IsEqualTypes(int_type1, lex.string) == "error") {
            throw std::logic_error("integer expected");
        }
        expCheck.Clear();
        if (_lex[_ind].string != "]") throw std::logic_error("\"]\" expected");
        type1 = GetArrayType(type1);
        gc();
    }
    while (_lex[_ind].string == ".") {
        if (def) throw std::logic_error("\"=\" expected");
        gc();
        if (_lex[_ind].type != "variable") {
            throw std::logic_error("method expected");
        }
        std::string method = _lex[_ind].string;
        gc();

        if (_lex[_ind].string == "(") {
            _parameter_arr = _tid->GetMethodParameters(type1, _lex[_ind - 1].string);
            parameters();
            _parameter_arr.clear();
            if (_lex[_ind].string != ")") {
                throw std::logic_error("\")\" expected");
            }
            gc();
            return;
        }

        type1 = _tid->GetMember(type1, method);


        if (_lex[_ind].string == "[") {
            gc();
            expCheck.Clear();
            expression();
            Lexeme lex = expCheck.GetType();
            std::string int_type1 = "int32", int_type2 = "int64";
            if (IsEqualTypes(int_type1, lex.string) == "error") {
                throw std::logic_error("integer expected");
            }
            expCheck.Clear();
            if (_lex[_ind].string != "]") throw std::logic_error("\"]\" expected");
            type1 = GetArrayType(type1);
            gc();
        }

    }

    if (_lex[_ind].string == "=") {
        gc();
        expression();
        Lexeme lex = expCheck.GetType();
        expCheck.Clear();
        if (IsEqualTypes(type1, lex.string) == "error") throw std::logic_error("trying to put " + lex.string + " into " +
        type1);
        return;
    }
    if (_lex[_ind].string == ";") {
        return;
    }
    throw std::logic_error("variable initialisation expected");

}

void SyntaxAnalyser::namepace() {
    if (_lex[_ind].string == "}") {
        //gc();
        return;
    }
    if (_lex[_ind].string == "array") {
        array_def();
        namepace();
        return;
    }
    if (_lex[_ind].type == "keyword" && !type()) {
        determinantes();
        namepace();
        return;
    }
    lexpression();
    if (_lex[_ind].string != ";") {
        throw std::logic_error("\";\" expected");
    }
    gc();
    if (_lex[_ind].string == "}") {
        return;
    }
    namepace();
    return;
}

void SyntaxAnalyser::functionDefinition() {
    is_in_function = true;
    if (!type()) {
        throw std::logic_error("type expected");
    }
    std::string f_type = _lex[_ind].string;
    gc();
    if (_lex[_ind].type != "variable") {
        throw std::logic_error("variable expected");
    }
    std::string f_name = _lex[_ind].string;
    gc();
    if (_lex[_ind].string != "(") {
        throw std::logic_error("\"(\" expected");
    }
    gc();
    //std::vector<std::pair<std::string, std::string>> formal_parameters;
    //should we make an array as a parameter?
    parameterDef();
    if (_lex[_ind].string != ")") {
        throw std::logic_error("\")\" expected");
    }
    if (!is_method) {
        _tid->AddFunction(f_name, _parameter_def_arr, f_type);
        _parameter_def_arr.clear();
    }
    else {
        _tid->AddMethod(_parameter_def_arr, method_name, method_return_type);
        _parameter_def_arr.clear();
    }
    gc();
    if (_lex[_ind].string != "{") {
        throw std::logic_error("\"{\" expected");
    }
    gc();
    _save_type = f_type;
    _tid->OpenScope();
    namepace();
    _tid->CloseScope();
    _save_type = "null";
    if (_lex[_ind].string != "}") {
        throw std::logic_error("\"}\" expected");
    }
    gc();
    if (!IAMRETURNINGSOMETHING && f_type != "void") {
        throw std::logic_error("function with non-void type must return something");
    }
    IAMRETURNINGSOMETHING = false;
    is_in_function = false;
}

void SyntaxAnalyser::type_cast() {
    if (!type()) throw std::logic_error("type expected");
    std::string type1 = _lex[_ind].string;
    gc();
    if (_lex[_ind].string != "cast") throw std::logic_error("\"cast\" expected");
    gc();
    std::string type2 = _tid->GetType(_lex[_ind].string);
    if (_lex[_ind].type != "variable") throw std::logic_error("variable expected");
    gc();
    if (!_tid->GetCast(type1, type2)) throw std::logic_error("no such cast exists");
}

void SyntaxAnalyser::type_cast_def() {
    is_in_function = true;
    if (!type()) throw std::logic_error("\"type\" expected");
    std::string cast_type_to = _lex[_ind].string;
    if (!_tid->IsTypeExist(cast_type_to)) throw std::logic_error("type " + cast_type_to + " does not exist");
    gc();
    if (_lex[_ind].string != "cast") throw std::logic_error("\"cast\" expected");
    gc();
    if (_lex[_ind].string != "(") throw std::logic_error("\"(\" expected");
    gc();
    if (!type()) throw std::logic_error("type expected");
    std::string cast_type_from = _lex[_ind].string;
    gc();
    if (!_tid->IsTypeExist(cast_type_from)) throw std::logic_error("type " + cast_type_from + " does not exist");
    variable();
    if (_lex[_ind].string != ")") throw std::logic_error("\")\" expected");
    gc();
    _tid->AddCast(cast_type_to, cast_type_from, _lex[_ind - 2].string);
    if (_lex[_ind].string != "{") throw std::logic_error("\"{\" expected");
    gc();
    //_tid->OpenScope();
    namepace();
    _tid->CloseScope();
    //problem with return
    if (_lex[_ind].string != "}") throw std::logic_error("\"}\" expected");
    gc();
    if (!IAMRETURNINGSOMETHING) {
        throw std::logic_error("cast must return something");
    }
    IAMRETURNINGSOMETHING = false;
    is_in_function = true;
}

void SyntaxAnalyser::print() {
    if (_lex[_ind].string != "(") throw std::logic_error("\"(\" expected");
    gc();
    if (_lex[_ind].type != "variable" && _lex[_ind].type != "string" &&
        _lex[_ind].type != "char" && _lex[_ind].type != "number") {
        throw std::logic_error("expression expected");
    }
    std::string type1;
    if (_lex[_ind].type == "variable") {
        type1 = _tid->GetType(_lex[_ind].string);
        if (_tid->IsTypeExist(type1) != 2) throw std::logic_error("bad type");
    }
    gc();
    if (_lex[_ind].string == ")") {
        gc();
        return;
    } else if (_lex[_ind].string != ",") {
        throw std::logic_error("\",\" expected");
    } else {
        do {
            gc();
            if (_lex[_ind].type != "variable" && _lex[_ind].type != "string" &&
                _lex[_ind].type != "char" && _lex[_ind].type != "number") {
                throw std::logic_error("expression expected");
            }
            if (_lex[_ind].type == "variable") {
                type1 = _tid->GetType(_lex[_ind].string);
                if (_tid->IsTypeExist(type1) != 2) throw std::logic_error("bad type");
            }
            gc();
        } while (_lex[_ind].string == ",");
        if (_lex[_ind].string != ")") throw std::logic_error("\")\" expected");
    }
    gc();
}

void SyntaxAnalyser::get() {
    if (_lex[_ind].string != "get") throw std::logic_error("\"get\" expected");
    gc();
    if (_lex[_ind].string != "(") throw std::logic_error("( expected");
    gc();
    input();
    if (_lex[_ind].string != ")") throw std::logic_error("( expected");
    gc();
}

void SyntaxAnalyser::input() {
    auto type1 = _tid->GetType(_lex[_ind].string);
    if (_tid->IsTypeExist(type1) != 2) throw std::logic_error("bad type");
    variable();
    while (_lex[_ind].string == ",") {
        gc();
        _tid->GetType(_lex[_ind].string);
        variable();
    }
}

void SyntaxAnalyser::If() {
    if (_lex[_ind].string != "(") throw std::logic_error("\"(\" expected");
    gc();
    expression();
    Lexeme lex = expCheck.GetType();
    expCheck.Clear();
    std::string b_string = "bool";
    if (IsEqualTypes(lex.string, b_string) == "error") {
        throw std::logic_error("bool type expected");
    }
    if (_lex[_ind].string != ")") throw std::logic_error("\")\" expected");
    gc();
    if (_lex[_ind].string != "{") throw std::logic_error("\"{\" expected");
    gc();
    _tid->OpenScope();
    namepace();
    _tid->CloseScope();
    if (_lex[_ind].string != "}") throw std::logic_error("\"}\" expected");
    gc();
    if (_lex[_ind].string != "else") return;
    gc();
    do {
        if (_lex[_ind].string == "if") {
            gc();
            if (_lex[_ind].string != "(") throw std::logic_error("\"(\" expected");
            gc();
            expression();
            Lexeme lex1 = expCheck.GetType();
            expCheck.Clear();
            if (IsEqualTypes(lex.string, b_string) == "error") {
                throw std::logic_error("bool type expected");
            }
            if (_lex[_ind].string != ")") throw std::logic_error("\")\" expected");
            gc();
        }
        if (_lex[_ind].string != "{") throw std::logic_error("\"{\" expected");
        gc();
        _tid->OpenScope();
        namepace();
        _tid->CloseScope();
        if (_lex[_ind].string != "}") throw std::logic_error("\"}\" expected");
        gc();
    } while (_lex[_ind].string == "else");
}

void SyntaxAnalyser::While() {
    if (_lex[_ind].string != "(") throw std::logic_error("\"(\" expected");
    gc();
    expression();
    Lexeme lex = expCheck.GetType();
    expCheck.Clear();
    std::string b_string = "bool";
    if (IsEqualTypes(lex.string, b_string) == "error") {
        throw std::logic_error("bool type expected");
    }
    if (_lex[_ind].string != ")") throw std::logic_error("\")\" expected");
    gc();
    if (_lex[_ind].string != "{") throw std::logic_error("\"{\" expected");
    gc();
    _tid->OpenScope();
    cycle_namespace();
    _tid->CloseScope();
}

void SyntaxAnalyser::dowhile() {
    if (_lex[_ind].string != "(") throw std::logic_error("\"(\" expected");
    gc();
    expression();
    Lexeme lex = expCheck.GetType();
    expCheck.Clear();
    std::string b_string = "bool";
    if (IsEqualTypes(lex.string, b_string) == "error") {
        throw std::logic_error("bool type expected");
    }
    if (_lex[_ind].string != ")") throw std::logic_error("\")\" expected");
    gc();
    if (_lex[_ind].string != "{") throw std::logic_error("\"{\" expected");
    gc();
    _tid->OpenScope();
    cycle_namespace();
    _tid->CloseScope();
}

void SyntaxAnalyser::For() {
    if (_lex[_ind].string != "(") throw std::logic_error("\"(\" expected");

    _tid->OpenScope();
    gc();
    //variable_def();
    // here must be var def + differentiation to (var : arr) and (var = .. ; .. ; ..)
    int diff = 0;
    std::string defined_variable;
    if (type()) {
        gc();
        if (type()) {
            _tid->IsTypeExist(_lex[_ind - 1].string);
            _tid->AddVariable(_lex[_ind - 1].string, _lex[_ind].string);
            defined_variable = _lex[_ind - 1].string;
            gc();
        } else {
            defined_variable = _tid->GetType(_lex[_ind - 1].string);
        }
        if (_lex[_ind].string == ":") diff = 1;
        else diff = 2;
    } else {
        throw std::logic_error("\";\" expected");
    }
    if (diff == 1) {
        gc();
        if (_lex[_ind].type != "variable") {
            throw std::logic_error("variable expected");
        }
        auto arr = _tid->GetType(_lex[_ind].string);
        auto type = GetArrayType(arr);
        if (IsEqualTypes(defined_variable, type) == "error") {
            throw std::logic_error("trying to put " + defined_variable + " into " + type);
        }
        //_tid->GetCast(type, defined_variable);
        /*if (IsEqualTypes(type, defined_variable) == "error") {
            throw std::logic_error("incorrect type of variable");
        }*/
        gc();

    } else if (diff == 2) {
        if (_lex[_ind].string == "=") {
            gc();
            expression();
            Lexeme lex1 = expCheck.GetType();
            expCheck.Clear();
            if (IsEqualTypes(lex1.string, defined_variable) == "error") {
                throw std::logic_error("trying to put " + lex1.string + " into " + defined_variable);
            }
        }
        if (_lex[_ind].string != ";") throw std::logic_error("; expected");
        gc();
        expression();
        Lexeme lex = expCheck.GetType();
        expCheck.Clear();
        std::string b_string = "bool";
        if (IsEqualTypes(lex.string, b_string) == "error") {
            throw std::logic_error("bool type expected");
        }
        if (_lex[_ind].string != ";") throw std::logic_error("\";\" expected");
        gc();
        lexpression();
    }
    if (_lex[_ind].string != ")") throw std::logic_error("\")\" expected");
    gc();
    if (_lex[_ind].string != "{") throw std::logic_error("\"{\" expected");
    gc();

    cycle_namespace();
    _tid->CloseScope();
}

void SyntaxAnalyser::variable() {
    if (_lex[_ind].type != "variable") throw std::logic_error("variable expected");
    gc();
}

void SyntaxAnalyser::variable_def() {
    if (!type()) throw std::logic_error("variable name expected");
    std::string var_type = _lex[_ind].string;
    gc();
    std::string var_name = _lex[_ind].string;
    variable();
    _tid->AddVariable(var_type, var_name);
    if (_lex[_ind].string == "(") {
        _parameter_arr = _tid->GetParameters(var_type);
        parameters();
        if (_lex[_ind].string != ")") throw std::logic_error(") expected");
        return;
    }
    if (_lex[_ind].string != "=") return;
    gc();
    expression();
    Lexeme lex = expCheck.GetType();
    expCheck.Clear();
    if (lex.string != var_type) throw std::logic_error("trying to put " + lex.string + " into " + var_type);
}

void SyntaxAnalyser::string() {
    if (_lex[_ind].string != "string" ||  _lex[_ind].type != "keywords") {
        throw std::logic_error("\"string\" expected");
    }
    gc();
    variable();
    if (_lex[_ind].string != "=") return;
    gc();
    if (_lex[_ind].type != "string") throw std::logic_error("string expected");
    gc();
}

void SyntaxAnalyser::array_def() {
    std::string type_name;
    if (_lex[_ind].string != "array")  {
        throw std::logic_error("\"array\" expected");
    }
    type_name += _lex[_ind].string;
    gc();
    if (!type()) throw std::logic_error("type expected");
    std::string arr_type = _lex[_ind].string;
    gc();
    std::string arr_name = _lex[_ind].string;
    auto add_name = "array " + arr_type;

    _tid->AddVariable(add_name, arr_name);
    variable();
    if (_lex[_ind].string == "(") {
        gc();
        expression();
        Lexeme lex = expCheck.GetType();
        expCheck.Clear();
        std::string int_type = "int32";
        if (IsEqualTypes(lex.string, int_type) == "error") {
            throw std::logic_error("int type expected");
        }
        if (_lex[_ind].string != ")") throw std::logic_error("\")\" expected");
        gc();
    }
    if (_lex[_ind].string != ";") throw std::logic_error("\";\" expected");
    gc();
}

void SyntaxAnalyser::cycle_namespace() {
    if (_lex[_ind].type == "keyword" && !type()) {
        if (_lex[_ind].string == "break" || _lex[_ind].string == "continue") {
            if (_lex[_ind].string != ";") throw std::logic_error("\";\" expected");
            cycle_namespace();
        } else {
            determinantes();
            cycle_namespace();
        }
    } else {
        if (_lex[_ind].string == "}") {
            gc();
            return;
        }
        lexpression();
        if (_lex[_ind].string != ";") {
            throw std::logic_error("\";\" expected");
        }
        gc();
        if (_lex[_ind].string == "}") {
            gc();
            return;
        }
        cycle_namespace();
        return;
    }
}

std::string SyntaxAnalyser::GetArrayType(std::string &s) {
    int i = 0;
    std::string tmp = "array";
    for (i = 0; i < 5; ++i) {
        if (tmp[i] != s[i]) throw std::logic_error("array expected");
    }
    while (i < s.size() && s[i] == ' ') ++i;
    std::string type;
    while (i < s.size()) {
        type += s[i];
        ++i;
    }
    return type;
}

std::string SyntaxAnalyser::IsEqualTypes(std::string& type1, std::string& type2) {
    // compare two types and return result type
    if (type1 == type2) return type1;
    if (_tid->IsTypeExist(type1) != 2 || _tid->IsTypeExist(type2) != 2) {
        if (_tid->GetCast(type1, type2)) return type1;
        if (_tid->GetCast(type2, type1)) return type2;
        return "error";
    }
    if (type1 == "int32" || type1 == "int64" || type1 == "float32" || type1 == "float64") {
        if (type2 == "string") return "error";
        if (type2 == "char") return type2;
    }
    if (type1 == "int32") {
        return type2;
    }
    if (type1 == "int64") {
        if (type2 == "float32" || type2 == "float64") return "float64";
        return type1;
    }
    if (type1 == "float32") {
        if (type2 == "int32") return type1;
        return "float64";
    }
    if (type1 == "float64") return type1;
    if (type1 == "char") {
        if (type2 != "string") return type2;
        return "error";
    }
    if (type1 == "string") {
        if (type2 == "char") return type1;
        return "error";
    }
    return "error";
}