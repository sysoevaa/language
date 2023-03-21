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
        //possible type check with else
        if (lexeme.string == "]") {
            _stack.pop_back();
        }
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

    /*if (_stack[_stack.size() - 2].type == "variable") {
        return;
    }*/

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

    if (_stack[_stack.size() - 2].type == "unary") {
        return;
    }

    std::string type1 = _stack[_stack.size() - 1].string, type2 = _stack[_stack.size() - 3].string;
    std::string oper = _stack[_stack.size() - 2].string;

    Lexeme lex("variable", _tid->GetTypeOverload(type1, type2, oper), 0);

    _stack.pop_back();
    _stack.pop_back();
    _stack.pop_back();
    Push(lex);
}

Lexeme ExpCheck::GetType() {
    return _stack[0];
}

void ExpCheck::TailMerge() {
    while (GetPriority() < 8) {
        Merge();
    }
}

void ExpCheck::Clear() {
    _stack.clear();
}

std::vector<Lexeme> ExpCheck::GetStack() {
    return _stack;
}

void ExpCheck::SetStack(std::vector<Lexeme> &stack) {
    _stack = stack;
}

Lexeme* ExpCheck::GetLast() {
    return &_stack[_stack.size() - 1];
}
