#include "Lexeme.h"

class SyntaxAnalyser {
    void gc() {

    }

    bool type() {
        if (_lex[_ind].type == 2) {
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
        return false;
    }

    //распихать унарки, обработка стрринга и буковок
    void expression() {
        if (_lex[_ind].type == 2 || _lex[_ind].type == 3) {
            gc();
            if (_lex[_ind].type == 6 && _lex[_ind].string == "(") {
                parameters();
                if (_lex[_ind].string != ")") {
                    throw ;
                }
                gc();
            }
            if (_lex[_ind].type == 4) {
                //распихать унарки в другой тип
                gc();
                expression();
            }
            return;
        }

        if (_lex[_ind].type == 6 && _lex[_ind].string == "(") {
            gc();
            expression();
            if (_lex[_ind].string != ")") {
                throw ;
            }
            gc();
            return;
        }

        throw;
    }

    void parameters() {
        do {
            gc();
            expression();
        } while (_lex[_ind].string == ',');
    }

    void determinantes() {
        if (_lex[_ind].string == "if") {

        }
        else if (_lex[_ind].string == "while") {

        }
        else if (_lex[_ind].string == "for") {

        }
        else if (_lex[_ind].string == "do") {
            gc();
            if (_lex[_ind].string == "while") {

            }
            throw;
        }
        else if (_lex[_ind].string == "while") {

        }
        else if (_lex[_ind].string == "print") {

        }
        else if (_lex[_ind].string == "get") {

        }
        else {
            throw;
        }
    }

    void lexpression() {
        bool def = false;
        if (_lex[_ind].type == 7) {
            gc();
            if (_lex[_ind].type != 2) {
                throw;
            }
            return;
        }
        if (type()) {
            def = true;
            gc();
        }
        if (_lex[_ind].type != 2) {
            throw;
        }
        gc();
        if (_lex[_ind].string == ".") {
            gc();
            if (_lex[_ind].type != 2) {
                throw;
            }
        }
        if (_lex[_ind].string == "(") {
            if (def) {
                throw;
            }
            parameters();
            if (_lex[_ind].string != ")") {
                throw;
            }
        }
        else if (_lex[_ind].string == "=") {
            gc();
            expression();
        }
        else {
            throw;
        }

    }

    void namepace() {
        if (_lex[_ind].type == 1 && !type()) {
            determinantes();
            gc();
            namepace();
            return;
        }
        lexpression();
        if (_lex[_ind].string != ";") {
            throw;
        }
        gc();
        namepace();
        return;
    }

    void functionDefinition() {
        if (!type()) {
            throw;
        }
        gc();
        if (_lex[_ind].type != 2) {
            throw;
        }
        gc();
        if (_lex[_ind].string != "(") {
            throw;
        }
        parameters();
        if (_lex[_ind].string != ")") {
            throw;
        }
        gc();
        if (_lex[_ind].string != "{") {
            throw;
        }
    }



private:
    std::vector<Lexeme> _lex;
    int _ind = 0;
};
