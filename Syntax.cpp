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
        gc();
        gc();
        if (_lex[_ind + 2].string == ";") {
            gc();
            gc();
            gc();
            globalNamespaceNoExec();
            return;
        }
        if (_lex[_ind].string == "=") {
            gc();
            expression();
            Lexeme lex = expCheck.GetType();
            expCheck.Clear();
            if (var_type != lex.string) throw std::logic_error("unexpected type");
            if (_lex[_ind].string == ";") gc();
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
    gc();
    if (_lex[_ind].string != "{") throw std::logic_error("\"{\" expected");
    gc();
    member();
    if (_lex[_ind].string != "}") throw std::logic_error("\"}\"expected");
    gc();
}

void SyntaxAnalyser::member() {
    if (_lex[_ind].string == "construct") {
        construct();
        member();
        return;
    }
    if (_lex[_ind].string == "array") {
        array_def();
        member();
        return;
    }
    if (type()) {
        std::string member_type = _lex[_ind].string;
        if (_ind + 2 >= _lex.size()) throw std::logic_error("unexpected end of file");
        if (_lex[_ind + 1].string == "overload") {
            overload();
            member();
            return;
        }
        else if (_lex[_ind + 1].type != "variable") throw std::logic_error("variable expected");
        std::string member_name = _lex[_ind + 1].type;
        if (_lex[_ind + 2].string == "(") {
            //how to add member?
            functionDefinition();
            member();
            return;
        }
        gc();
        gc();
        if (_lex[_ind].string == ";") {
            gc();
            member();
            return;
        }
        if (_lex[_ind].string == "=") {
            gc();
            expression();
            Lexeme lex = expCheck.GetType();
            expCheck.Clear();
            if (lex.string != member_type) throw std::logic_error("unexpected type");
            member();
            return;
        }
        throw std::logic_error("unexpected symbols");
    }
    return;
}

void SyntaxAnalyser::construct() {
    //blyaaa eshe constructori suka
    if (_lex[_ind].string != "construct") throw std::logic_error("\"construct \" expected");
    gc();
    if (_lex[_ind].string != "(") throw std::logic_error("\"(\" expected");
    gc();
    parameterDef();
    if (_lex[_ind].string != ")") throw std::logic_error("\")\" expected");
    gc();
    if (_lex[_ind].string != "{") throw std::logic_error("\"{\" expected");
    gc();
    _tid->OpenScope();
    namepace();
    _tid->CloseScope();
    if (_lex[_ind].string != "}") throw std::logic_error("\"}\" expected");
    gc();
}

void SyntaxAnalyser::overload() {
    if (!type()) throw std::logic_error("type of function expected");
    std::string type1 = _lex[_ind].string;
    gc();
    if (_lex[_ind].string != "overload") throw std::logic_error("\"overload\" expected");
    gc();
    if (_lex[_ind].type != "binary") throw std::logic_error("binary operator expected");
    std::string operator_ = _lex[_ind].string;
    gc();
    if (_lex[_ind].string != "(") throw std::logic_error("\"( \" expected");
    gc();
    std::string type2;
    //how to check second parameter type?
    parameterDef();
    if (_lex[_ind].string != ")") throw std::logic_error("\")\" expected");
    _tid->AddOverload(type1, operator_, type2);
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
        if (_lex[_ind].string == ".") {
            gc();
            variable();
            std::string type_name = _tid->GetType(_lex[_ind - 3].string);
            expCheck.Process(Lexeme("variable",  _tid->GetMember(type_name, _lex[_ind].string), 0));
        }
        else {
            expCheck.Process(Lexeme("variable", _tid->GetType(_lex[_ind - 1].string), 0));
        }
        //ask! what did I mean by this my god
        if (_lex[_ind].string == "(") {
            parameters();
            if (_lex[_ind].string != ")") {
                throw std::logic_error("expected \")\"");
            }
            gc();
        }

        if (_lex[_ind].type == "binary" || _lex[_ind].type == "power" || _lex[_ind].type == "bool") {
            expCheck.Process(_lex[_ind]);
            gc();
            expression();
        }
        if (_lex[_ind].string == "[") {
            expCheck.Process(_lex[_ind]);
            gc();
            expression();
            if (_lex[_ind].string != "]") {
                throw std::logic_error ("] expected");
            }
            expCheck.Process(_lex[_ind]);
            gc();
        }
        expCheck.TailMerge();
        return;
    }

    if (type()) {
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

    if (_lex[_ind].type == "string") {
        expCheck.Process(_lex[_ind]);
        gc();
        return;
    }

    if (_lex[_ind].type == "unary") {
        expCheck.Process(_lex[_ind]);
        gc();
        if (_lex[_ind].type == "variable" || _lex[_ind].type == "number" || _lex[_ind].type == "char") {
            if (_lex[_ind].string == ".") {
                gc();
                variable();
                std::string type_name = _tid->GetType(_lex[_ind - 3].string);
                expCheck.Process(Lexeme("variable",  _tid->GetMember(type_name, _lex[_ind].string), 0));
            }
            else {
                expCheck.Process(Lexeme("variable", _tid->GetType(_lex[_ind - 1].string), 0));
            }
            //ask! ask what??
            expCheck.Process(Lexeme("variable", _tid->GetType(_lex[_ind - 1].string), 0));
            if (_lex[_ind].type == "binary" || _lex[_ind].type == "power" || _lex[_ind].type == "bool") {
                expCheck.Process(_lex[_ind]);
                gc();
                expression();
            }
            if (_lex[_ind].string == "[") {
                expCheck.Process(_lex[_ind]);
                gc();
                expression();
                if (_lex[_ind].string != "]") {
                    throw std::logic_error ("] expected");
                }
                expCheck.Process(_lex[_ind]);
                gc();
            }
            expCheck.TailMerge();
            return;
        }

        if (_lex[_ind].string == "(") {
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

    }
    throw std::logic_error("unexpected symbols");
}

void SyntaxAnalyser::parameters() {
    //need to access function name
    std::vector<std::string> parameter_types;
    int i = 0;
    do {
        gc();
        if (_lex[_ind].string == ")") return;
        expression();
        Lexeme lex = expCheck.GetType();
        expCheck.Clear();
        if (lex.string != parameter_types[i]) throw std::logic_error("parameter types don't match");
    } while (_lex[_ind].string == ",");
}

void SyntaxAnalyser::parameterDef() {
    //array as a parameter maybe?
    if (_lex[_ind].string == ")") return;
    do {
        if (_lex[_ind - 1].string != "(") gc();
        if (!type()) {
            throw std::logic_error("expected type name");
        }
        gc();
        if (_lex[_ind].type != "variable") {
            throw std::logic_error("variable expected");
        }
        gc();
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
        gc();
        get();
        if (_lex[_ind].string != ";") {
            throw std::logic_error("\";\" expected");
        }
        gc();
    } else if (_lex[_ind].string == "return") {
        gc();
        if (_lex[_ind].string != ";") {
            expression();
            Lexeme lex = expCheck.GetType();
            expCheck.Clear();
            //Check, somehow we need to access the function type
        }
        if (_lex[_ind].string != ";") {
            throw std::logic_error("\";\" expected");
        }
        gc();
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
    if (type()) {
        //check if struct exists
        gc();
        if (_lex[_ind].type == "variable") {
            _tid->AddVariable(_lex[_ind - 1].string, _lex[_ind - 1].string);
            def = true;
            gc();
        } else if (_lex[_ind - 1].type == "keyword") {
            throw std::logic_error("variable name expected");
        }
    } else {
        throw std::logic_error("name expected");
    }
    //gc();
    if (_lex[_ind].string == "[") {
        gc();
        if (_lex[_ind].type != "number" || _lex[_ind].type != "variable") throw std::logic_error("index expected");
        gc();
        if (_lex[_ind].string != "]") throw std::logic_error("\"]\" expected");
        gc();
    }
    if (_lex[_ind].string == ".") {
        gc();
        if (_lex[_ind].type != "variable") {
            throw std::logic_error("method expected");
        }
        gc();
    }
    if (_lex[_ind].string == "(") {
        if (def) {
            throw std::logic_error("\"=\" expected");
        }
        parameters();
        if (_lex[_ind].string != ")") {
            throw std::logic_error("\")\" expected");
        }
        gc();
        return;
    }
    if (_lex[_ind].string == "=") {
        gc();
        expression();
        Lexeme lex = expCheck.GetType();
        expCheck.Clear();
        //Check
        return;
    }
    if (_lex[_ind].string == ";") {
        return;
    }
    throw std::logic_error("unexpected symbol");

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
    std::vector<std::pair<std::string, std::string>> formal_parameters;
    //should we make an array as a parameter?
    parameterDef();
    if (_lex[_ind].string != ")") {
        throw std::logic_error("\")\" expected");
    }
    //no function type included
    _tid->AddFunction(f_name, formal_parameters);
    gc();
    if (_lex[_ind].string != "{") {
        throw std::logic_error("\"{\" expected");
    }
    gc();
    _tid->OpenScope();
    namepace();
    _tid->CloseScope();
    if (_lex[_ind].string != "}") {
        throw std::logic_error("\"}\" expected");
    }
    gc();
}

void SyntaxAnalyser::type_cast() {
    if (!type()) throw std::logic_error("type expected");
    std::string type1 = _lex[_ind].string;
    gc();
    if (_lex[_ind].string != "cast") throw std::logic_error("\"cast\" expected");
    gc();
    std::string type2 = _lex[_ind].string;
    if (_lex[_ind].type != "variable") throw std::logic_error("variable expected");
    gc();
    if (!_tid->GetCast(type1, type2)) throw std::logic_error("no such cast exists");
}

void SyntaxAnalyser::type_cast_def() {
    if (!type()) throw std::logic_error("\"type\" expected");
    std::string cast_type_to = _lex[_ind].string;
    gc();
    if (_lex[_ind].string != "cast") throw std::logic_error("\"cast\" expected");
    gc();
    if (_lex[_ind].string != "(") throw std::logic_error("\"(\" expected");
    gc();
    if (!type()) throw std::logic_error("type expected");
    gc();
    std::string cast_type_from = _lex[_ind].string;
    variable();
    if (_lex[_ind].string != ")") throw std::logic_error("\")\" expected");
    gc();
    if (_lex[_ind].string != "{") throw std::logic_error("\"{\" expected");
    gc();
    _tid->OpenScope();
    namepace();
    _tid->CloseScope();
    //problem with return
    if (_lex[_ind].string != "}") throw std::logic_error("\"}\" expected");
    gc();
}

void SyntaxAnalyser::print() {
    if (_lex[_ind].string != "(") throw std::logic_error("\"(\" expected");
    gc();
    if (_lex[_ind].type != "variable" && _lex[_ind].type != "string" &&
        _lex[_ind].type != "char" && _lex[_ind].type != "number") {
        throw std::logic_error("expression expected");
    }
    gc();
    if (_lex[_ind].string == ")") {
        gc();
        return;
    } else if (_lex[_ind].string != ",") {
        throw std::logic_error("\",\" expected");
    } else {
        gc();
        do {
            if (_lex[_ind].type != "variable" || _lex[_ind].type != "string" ||
                _lex[_ind].type != "char" || _lex[_ind].type != "number") {
                throw std::logic_error("expression expected");
            }
            gc();
        } while (_lex[_ind].string == ",");
        gc();
        if (_lex[_ind].string != ")") throw std::logic_error("\")\" expected");
    }
    gc();
}

void SyntaxAnalyser::get() {
    if (_lex[_ind].string != "get") throw std::logic_error("\"get\" expected");
    gc();
    input();
}

void SyntaxAnalyser::input() {
    variable();
    while (_lex[_ind].string == ",") {
        gc();
        variable();
    }
}

void SyntaxAnalyser::If() {
    if (_lex[_ind].string != "(") throw std::logic_error("\"(\" expected");
    gc();
    expression();
    Lexeme lex = expCheck.GetType();
    expCheck.Clear();
    if (lex.string != "bool" && lex.string != "int32" && lex.string != "int64") {
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
            if (lex1.string != "bool" && lex1.string != "int32" && lex1.string != "int64") {
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
    if (lex.string != "bool" && lex.string != "int32" && lex.string != "int64") {
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
    if (lex.string != "bool" && lex.string != "int32" && lex.string != "int64") {
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
    gc();
    variable_def();
    if (_lex[_ind].string == ":") {
        gc();
        if (_lex[_ind].type != "variable") {
            throw std::logic_error("variable expected");
        }
    } else if (_lex[_ind].string == ";") {
        gc();
        expression();
        Lexeme lex = expCheck.GetType();
        expCheck.Clear();
        if (lex.string != "bool" && lex.string != "int32" && lex.string != "int64") {
            throw std::logic_error("bool type expected");
        }
        if (_lex[_ind].string != ";") throw std::logic_error("\";\" expected");
        gc();
        lexpression();
    } else {
        throw std::logic_error("\";\" expected");
    }
    if (_lex[_ind].string != ")") throw std::logic_error("\")\" expected");
    gc();
    if (_lex[_ind].string != "{") throw std::logic_error("\"{\" expected");
    gc();
    _tid->OpenScope();
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
    if (_lex[_ind].string != "array")  {
        throw std::logic_error("\"array\" expected");
    }
    gc();
    if (!type()) throw std::logic_error("type expected");
    //add array
    std::string arr_type = _lex[_ind].string;
    gc();
    std::string arr_name = _lex[_ind].string;
    _tid->AddVariable(arr_type, arr_name);
    variable();
    if (_lex[_ind].string == "(") {
        gc();
        expression();
        Lexeme lex = expCheck.GetType();
        expCheck.Clear();
        if (lex.string != arr_type) throw std::logic_error("trying to put " + lex.string + " into " + arr_type);
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
