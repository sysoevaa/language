#include "Lexeme.h"

class SyntaxAnalyser {
    void gc() {

    }
    //распихать унарки, обработка стрринга и буковок
    void expression() {
        gc();
        if (_current->type == 2 || _current->type == 3) {
            gc();
            if (_current->type == 6 && _current->string == "(") {
                parameters();
                gc();
                if (_current->string != ")") {
                    throw ;
                }
                gc();
            }
            if (_current->type == 4) {
                //распихать унарки в другой тип
                gc();
                expression();
            }
            return;
        }

        if (_current->type == 6 && _current->string == "(") {
            expression();
            gc();
            if (_current->string != ")") {
                throw ;
            }
            gc();
            return;
        }

        throw;

        //if (_current->type == )
        //если второй тип - универсальная функция проверки на имя переменной/вызов функции
        //обязательная проверка на пустоту / оператор

        //вызов еще одной функции экспрешон(если флаг сработал)?
        //или же в конце чекаем, есть ли точка с запятой, после чего завершаем
    }
    void parameters() {

    }

private:
    std::vector<Lexeme> _lexemes;
    Lexeme* _current = nullptr;
};
