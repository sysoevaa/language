#include "Lexeme.h"

class SyntaxAnalyser {
  void gc() {
     ++_ind;
     if (_ind >= _lex.size()) throw;
  }

  bool type() {
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
      return false;
  }

  void program() {
      gc();
      globalNamespaceNoExec();
      if (_lex[_ind].string != "exec") throw;
      gc();
      functionDefinition();
      gc();
      globalNamespace();
  }

  void globalNamespaceNoExec() {
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
          if (_ind + 2 >= _lex.size()) throw;
          if (_lex[_ind + 1].string == "cast") {
              type_cast_def();
              globalNamespaceNoExec();
              return;
          }
          else if (_lex[_ind + 1].type != "variable") throw;
          if (_lex[_ind + 2].string == "(") {
              functionDefinition();
              globalNamespaceNoExec();
              return;
          }
          gc();
          gc();
          if (_lex[_ind + 2].string == ";") {
              gc();
              globalNamespaceNoExec();
              return;
          }
          if (_lex[_ind + 2].string == "=") {
              gc();
              expression();
              globalNamespaceNoExec();
              return;
          }
          throw;
      }
      return;
  }

  void globalNamespace() {
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
          if (_ind + 2 >= _lex.size()) throw;
          if (_lex[_ind + 1].string == "cast") {
              type_cast_def();
              globalNamespace();
              return;
          }
          else if (_lex[_ind + 1].type != "variable") throw;
          if (_lex[_ind + 2].string == "(") {
              functionDefinition();
              globalNamespace();
              return;
          }
          gc();
          gc();
          if (_lex[_ind + 2].string == ";") {
              gc();
              globalNamespace();
              return;
          }
          if (_lex[_ind + 2].string == "=") {
              gc();
              expression();
              globalNamespace();
              return;
          }
          throw;
      }
      return;
  }

  void object() {
      if (_lex[_ind].string != "struct") throw;
      gc();
      if (_lex[_ind].type != "variable") throw;
      if (_lex[_ind].type != "{") throw;
      member();
      if (_lex[_ind].string != "}") throw;
      gc();
  }

  void member() {
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
          if (_ind + 2 >= _lex.size()) throw;
          if (_lex[_ind + 1].string == "overload") {
              overload();
              member();
              return;
          }
          else if (_lex[_ind + 1].type != "variable") throw;
          if (_lex[_ind + 2].string == "(") {
              functionDefinition();
              member();
              return;
          }
          gc();
          gc();
          if (_lex[_ind + 2].string == ";") {
              gc();
              member();
              return;
          }
          if (_lex[_ind + 2].string == "=") {
              gc();
              expression();
              member();
              return;
          }
          throw;
      }
      return;
  }

  void construct() {
      if (_lex[_ind].string != "construct") throw;
      gc();
      if (_lex[_ind].string != "(") throw;
      gc();
      parameterDef();
      if (_lex[_ind].string != ")") throw;
      gc();
      if (_lex[_ind].string != "{") throw;
      gc();
      namepace();
      if (_lex[_ind].string != "}") throw;
      gc();
  }

  void overload() {
      if (!type()) throw;
      gc();
      if (_lex[_ind].string != "overload") throw;
      gc();
      if (_lex[_ind].type != "binary") throw;
      gc();
      if (_lex[_ind].string != "(") throw;
      gc();
      parameterDef();
      if (_lex[_ind].string != ")") throw;
      gc();
      if (_lex[_ind].string != "{") throw;
      gc();
      namepace();
      if (_lex[_ind].string != "}") throw;
  }

  void expression() {
      if (_lex[_ind].type == "variable" || _lex[_ind].type == "number" || _lex[_ind].type == "char") {
          gc();
          if (_lex[_ind].type == "bracket" && _lex[_ind].string == "(") {
              parameters();
              if (_lex[_ind].string != ")") {
                  throw;
              }
              gc();
          }
          if (_lex[_ind].type == "binary") {
              gc();
              expression();
          }
          return;
      }

      if (_lex[_ind].type == "bracket" && _lex[_ind].string == "(") {
          gc();
          expression();
          if (_lex[_ind].string != ")") {
              throw;
          }
          gc();
          return;
      }

      if (_lex[_ind].type == "string") {
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
          if (_lex[_ind].type != "variable") {
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
          gc();
          For();
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
      if (_lex[_ind].type == "unary") {
          gc();
          if (_lex[_ind].type != "variable") {
              throw;
          }
          return;
      }
      if (type()) {
          def = true;
          gc();
      }
      if (_lex[_ind].type != "variable") {
          throw;
      }
      gc();
      if (_lex[_ind].string == "[") {
          gc();
          if (_lex[_ind].type != "number") throw;
          gc();
          if (_lex[_ind].string != "[") throw;
      }
      if (_lex[_ind].string == ".") {
          gc();
          if (_lex[_ind].type != "variable") {
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
      if (_lex[_ind].string == "array") {
          array_def();
          namepace();
          return;
      }
      if (_lex[_ind].type == "keyword" && !type()) {
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
      if (_lex[_ind].type != "variable") {
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
      if (_lex[_ind].type != "variable") throw;
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
      if (_lex[_ind].type != "variable" || _lex[_ind].type != "string" ||
      _lex[_ind].type != "char" || _lex[_ind].type != "number") {
          throw;
      }
      gc();
      if (_lex[_ind].string == ")") {
          gc();
          return;
      } else if (_lex[_ind].string != ",") {
          throw;
      } else {
          gc();
          do {
              if (_lex[_ind].type != "variable" || _lex[_ind].type != "string" ||
                  _lex[_ind].type != "char" || _lex[_ind].type != "number") {
                  throw;
              }
              gc();
          } while (_lex[_ind].string == ",");
          gc();
          if (_lex[_ind].string != ")") throw;
      }
      gc();
  }

  void get() {
      if (_lex[_ind].string != "get") throw;
      gc();
      input();
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
          if (_lex[_ind].type != 10) throw;
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
          gc();
          if (_lex[_ind].type != "variable") {
              throw;
          }
      } else if (_lex[_ind].string == ";") {
          gc();
          bool_expression();
          if (_lex[_ind].string != ";") throw;
          gc();
          expression();
      } else {
          throw;
      }
      if (_lex[_ind].string != ")") throw;
      gc();
      if (_lex[_ind].string != "{") throw;
      gc();
      cycle_namespace();
  }

  void variable() {
    if (_lex[_ind].type != "variable") throw;
    gc();
  }

  void variable_def() {
    if (!type()) throw;
    gc();
    variable();
    if (_lex[_ind].string != "=") return;
    gc();
    expression();
  }

  void string() {
      if (_lex[_ind].string != "string" ||  _lex[_ind].type != "keywords") throw;
      gc();
      variable();
      if (_lex[_ind].string != "=") return;
      gc();
      if (_lex[_ind].type != "string") throw;
      gc();
  }

  void array_def() {
      if (_lex[_ind].string != "array" || _lex[_ind].type != "keywords") throw;
      gc();
      if (!type()) throw;
      gc();
      variable();
      if (_lex[_ind].string != "(") return;
      gc();
      expression();
      if (_lex[_ind].string != ")") throw;
      gc();
  }

  void cycle_namespace() {
      if (_lex[_ind].type == "keywords") {
          if (_lex[_ind].string == "break" || _lex[_ind].string == "continue") {
              if (_lex[_ind].string != ";") throw;
              cycle_namespace();
          } else {
              determinantes();
              cycle_namespace();
          }
      } else {
          lexpression();
          if (_lex[_ind].string != ";") {
              throw;
          }
          gc();
          if (_lex[_ind].string == "}") {
              return;
          }
          cycle_namespace();
          return;
      }
  }

  private:
  std::vector<Lexeme> _lex;
  int _ind = 0;
};
