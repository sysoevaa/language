#include "Lexeme.h"
#include "ExpCheck.h"
#include <string>

void ExpCheck::Process(Lexeme lexeme) {
    if (lexeme.type == "binary" || lexeme.type == "power" || lexeme.type == "bool") {
        while (ExpCheck::GetPriority() <= lexeme.priority) {
            Merge();
        }
        Push(lexeme);
        return;
    }

    if (lexeme.string == "(" || lexeme.string == "[") {
        Push(lexeme);
        return;
    }

    if (lexeme.string == ")" || lexeme.string == "]") {
        while (GetPriority() != 8) {
            Merge();
        }
        Push(lexeme);
        Merge();
        return;
    }

    if (lexeme.type == "variable") {
        Push(lexeme);
        return;
    }
}

void ExpCheck::Push(Lexeme lexeme) {
    _stack.push_back(lexeme);
}

int ExpCheck::GetPriority() {
    if (_stack.size() < 2) {
        return 9;
    }
    if (_stack[_stack.size() - 2].type == "unary") {
        return -1;
    }
    if (_stack[_stack.size() - 2].type == "binary" || _stack[_stack.size() - 2].type == "power" ||
    _stack[_stack.size() - 2].type == "bool") {
        return _stack[_stack.size() - 2].priority;
    }

    if (_stack[_stack.size() - 2].string == "cast") {
        return 0;
    }

    if (_stack[_stack.size() - 2].string == "(" || _stack[_stack.size() - 2].string == "[") {
        return 8;
    }

    return 9;
}

void ExpCheck::Merge() {
    if (_stack[_stack.size() - 1].string == ")" || _stack[_stack.size() - 1].string == "]") {
        _stack.pop_back();
        Lexeme lex = _stack[_stack.size() - 1];
        _stack.pop_back();
        _stack.pop_back();
        Push(lex);
        return;
    }

    if (_stack[_stack.size() - 2].string == "cast") {
        _stack.pop_back();
        _stack.pop_back();
        Lexeme lex = _stack[_stack.size() - 1];
        _stack.pop_back();
        Push(lex);
        return;
    }

    std::string type1 = _stack[_stack.size() - 1].string, type2 = _stack[_stack.size() - 3].string;
    std::string oper = _stack[_stack.size() - 1].string;



//    if (type1 == "string" || type2 == "string") {
//        throw std::logic_error("can't fit string in expression");
//    }
//
//    if (type1 == "int32" && type2 == "int64") {
//        Lexeme lex = _stack[_stack.size() - 3];
//        _stack.pop_back();
//        _stack.pop_back();
//        _stack.pop_back();
//        Push(lex);
//        return;
//    }
//
//    if (type1 == "int32" && type2 == "int64") {
//        Lexeme lex = _stack[_stack.size() - 3];
//        _stack.pop_back();
//        _stack.pop_back();
//        _stack.pop_back();
//        Push(lex);
//        return;
//    }
//
//    if (type1 == "int64" && type2 == "int32") {
//        Lexeme lex = _stack[_stack.size() - 1];
//        _stack.pop_back();
//        _stack.pop_back();
//        _stack.pop_back();
//        Push(lex);
//        return;
//    }
//
//    if (type1 == "int32" && type2 == "int64") {
//        Lexeme lex = _stack[_stack.size() - 3];
//        _stack.pop_back();
//        _stack.pop_back();
//        _stack.pop_back();
//        Push(lex);
//        return;
//    }
}

Lexeme ExpCheck::GetType() {
    return _stack[0];
}

void ExpCheck::TailMerge() {
    while (GetPriority() < 8) {
        Merge();
    }
}

void Clear() {
    _stack.clear();
}
