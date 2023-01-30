#include "Lexeme.h"

class SyntaxAnalyser {
    void gc() {

    }

    void type() {
        if (_lex[_ind].type == 2) {
            gc();
            return;
        }
        if (_lex[_ind].string == "") {

        }
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
        //if (_ind->type == )
        //если второй тип - универсальная функция проверки на имя переменной/вызов функции
        //обязательная проверка на пустоту / оператор

        //вызов еще одной функции экспрешон(если флаг сработал)?
        //или же в конце чекаем, есть ли точка с запятой, после чего завершаем
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
