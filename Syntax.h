#include "Lexeme.h"

class SyntaxAnalyser {
  void gc() {

  }

  bool type() {
      if (_lex[_ind].type == 2) { // почему
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

  void program() {
      gc();
      globalNamepaceNoExec();
      if (_lex[_ind].string != "exec") throw;
      gc();
      functionDefinition();
      gc();
      globalNamespace();
  }

  void globalNamepaceNoExec() {

  }

  void globalNamespace() {

  }

  //распихать унарки, обработка стрринга и буковок
  void expression() {
      if (_lex[_ind].type == 2 || _lex[_ind].type == 3) {
          gc();
          if (_lex[_ind].type == 6 && _lex[_ind].string == "(") {
              parameters();
              if (_lex[_ind].string != ")") {
                  throw;
              }
              gc();
          }
          if (_lex[_ind].type == 4) {
              gc();
              expression();
          }
          return;
      }

      if (_lex[_ind].type == 6 && _lex[_ind].string == "(") {
          gc();
          expression();
          if (_lex[_ind].string != ")") {
              throw;
          }
          gc();
          return;
      }

      if (_lex[_ind].type == 9 || ) {
          return;
      }

      throw;
  }

  void parameters() {
      do {
          gc();
          expression();
      } while (_lex[_ind].string == ",");
  }

  void parameterDef() {
      do {
          if (!type()) {
              throw;
          }
          gc();
          if (_lex[_ind].type != 2) {
              throw;
          }
          gc();
      } while (_lex[_ind].string == ",");
  }

  void determinantes() {
      if (_lex[_ind].string == "if") {
          gc();
          If();
      } else if (_lex[_ind].string == "while") {
          gc();
          While();
      } else if (_lex[_ind].string == "for") {

      } else if (_lex[_ind].string == "do") {
          gc();
          if (_lex[_ind].string == "while") {
              gc();
            dowhile();
          }
          throw;
      } else if (_lex[_ind].string == "print") {
          gc();
          print();
          if (_lex[_ind].string != ";") {
              throw;
          }
          gc();
      } else if (_lex[_ind].string == "get") {
          gc();
          get();
          if (_lex[_ind].string != ";") {
              throw;
          }
          gc();
      } else if (_lex[_ind].string == "return") {
          gc();
          if (_lex[_ind].string != ";") {
              expression();
          }
          if (_lex[_ind].string != ";") {
              throw;
          }
          gc();
      } else {
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
          return;
      }
      if (_lex[_ind].string == "=") {
          gc();
          expression();
          return;
      }
      throw;

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
      if (_lex[_ind].string == "}") {
          return;
      }
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
      parameterDef();
      if (_lex[_ind].string != ")") {
          throw;
      }
      gc();
      if (_lex[_ind].string != "{") {
          throw;
      }
      namepace();
      if (_lex[_ind].string != "}") {
          throw;
      }
  }

  void type_cast() {
      if (!type()) throw;
      gc();
      if (_lex[_ind].string != "cast") throw;
      gc();
      if (_lex[_ind].type != 2) throw;
      gc();
  }

  void type_cast_def() {
      if (!type()) throw;
      gc();
      if (_lex[_ind].string != "cast") throw;
      gc();
      if (!type()) throw;
      gc();
      variable();
      if (_lex[_ind].string != "{") throw;
      gc();
      namepace();
      if (_lex[_ind].string != "}") throw;
      gc();
  }

  void print() {
      if (_lex[_ind].string != "(") throw;
      gc();
      expression();
      if (_lex[_ind].string == ",") {
          do {
              gc();
              expression();
          } while (_lex[_ind].string == ",");
          gc();
      }
      if (_lex[_ind].string != ")") throw;
      gc();
  }

  void get() {
      if (_lex[_ind].string != "(") throw;
      input();
      if (_lex[_ind].string != ")") throw;
      gc();
  }

  void input() {
      variable();
      while (_lex[_ind].string == ",") {
          gc();
          variable();
      }
  }

  void bool_expression() {
      if (_lex[_ind].string == "(") {
          gc();
          bool_expression();
          if (_lex[_ind].string != ")") throw;
      } else if (_lex[_ind].type == "variable") {
          gc();
          if (_lex[_ind].type != "bool") throw;
          gc();
          if (_lex[_ind].string == "(") {
              gc();
              bool_expression();
              if (_lex[_ind].string != ")") throw;
              gc();
          } else if (_lex[_ind].type != "variable") throw;
      }
      gc();
  }

  void If() {
      if (_lex[_ind].string != "(") throw;
      gc();
      bool_expression();
      if (_lex[_ind].string != ")") throw;
      gc();
      if (_lex[_ind].string != "{") throw;
      gc();
      namepace();
      if (_lex[_ind].string != "else") return;
      gc();
      do {
          if (_lex[_ind].string == "if") {
              gc();
              if (_lex[_ind].string != "(") throw;
              gc();
              bool_expression();
              if (_lex[_ind].string != ")") throw;
              gc();
          }
          if (_lex[_ind].string != "{") throw;
          gc();
          namepace();
          if (_lex[_ind].string != "}") throw;
          gc();
      } while (_lex[_ind].string == "else");
      gc();
  }

  void While() {
      if (_lex[_ind].string != "(") throw;
      gc();
      bool_expression();
      if (_lex[_ind].string != ")") throw;
      gc();
      if (_lex[_ind].string != "{") throw;
      gc();
      namepace();
      if (_lex[_ind].string != "}") throw;
      gc();
  }

  void dowhile() {
      if (_lex[_ind].string != "(") throw;
      gc();
      bool_expression();
      if (_lex[_ind].string != ")") throw;
      gc();
      if (_lex[_ind].string != "{") throw;
      gc();
      namepace();
      if (_lex[_ind].string != "{") throw;
      gc();
  }

  void For() {
      if (_lex[_ind].string != "(") throw;
      gc();
      variable_def();
      if (_lex[_ind].string == ":") {
          variable();
      } else if (_lex[_ind].string == ";") {
          gc();
          bool_expression();
          if (_lex[_ind].string != ";") throw;
          expression();
      } else {
          throw;
      }
      if (_lex[_ind].string != "{") throw;
      gc();
      cycle_namespace();
      if (_lex[_ind].string != "}") throw;
      gc();
  }

  void variable() {
      if (_lex[_ind].type != "variable") throw;
      gc();
  }

  void variable_def() {
      if (!type()) throw;
      gc();
      if (_lex[_ind].type != "variable") throw;
      gc();
      if (_lex[_ind].string != "=") return;
      gc();
      expression();
  }

 private:
  std::vector<Lexeme> _lex;
  int _ind = 0;
};
