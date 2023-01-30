#include "Lexeme.h"

class SyntaxAnalyser {
    void gc() {

    }

    void type() {
        if (_lex[_ind].type == 2) {
            gc();
            return;
        }
        if (_lex[_ind].string == "char") {
            gc();
            return;
        }
        if (_lex[_ind].string == "int32") {
            gc();
            return;
        }
        if (_lex[_ind].string == "int64") {
            gc();
            return;
        }
        if (_lex[_ind].string == "float32") {
            gc();
            return;
        }
        if (_lex[_ind].string == "float64") {
            gc();
            return;
        }
        if (_lex[_ind].string == "string") {
            gc();
            return;
        }
        throw;
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

    void functionDefinition() {

    }



private:
    std::vector<Lexeme> _lex;
    int _ind = 0;
};
