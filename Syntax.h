#include "Lexeme.h"

class SyntaxAnalyser {
 public:
  SyntaxAnalyser (const std::vector<Lexeme>& lex) : _lex(lex) {}

  void gc() {
     ++_ind;
     if (_ind >= _lex.size()) throw std::logic_error("unexpected end of file");
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
      if (_lex[_ind].string != "exec") throw std::logic_error("\"exec\" exec");
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
          if (_ind + 2 >= _lex.size()) throw std::logic_error("unexpected end of file");
          if (_lex[_ind + 1].string == "cast") {
              type_cast_def();
              globalNamespaceNoExec();
              return;
          }
          else if (_lex[_ind + 1].type != "variable") throw std::logic_error("expected variable");
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
          throw std::logic_error("unexpected symbols");
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
          throw std::logic_error("unexpected symbols");
      }
      return;
  }

  void object() {
      if (_lex[_ind].string != "struct") throw std::logic_error("\"struct\" expected");
      gc();
      if (_lex[_ind].type != "variable") throw std::logic_error("variable expected");
      if (_lex[_ind].type != "{") throw std::logic_error("\"{\" expected");
      member();
      if (_lex[_ind].string != "}") throw std::logic_error("\"}\"expected");
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
          if (_ind + 2 >= _lex.size()) throw std::logic_error("unexpected end of file");;
          if (_lex[_ind + 1].string == "overload") {
              overload();
              member();
              return;
          }
          else if (_lex[_ind + 1].type != "variable") throw std::logic_error("variable expected");
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
          throw std::logic_error("unexpected symbols");
      }
      return;
  }

  void construct() {
      if (_lex[_ind].string != "construct") throw std::logic_error("\"construct \" expected");
      gc();
      if (_lex[_ind].string != "(") throw std::logic_error("\"(\" expected");
      gc();
      parameterDef();
      if (_lex[_ind].string != ")") throw std::logic_error("\")\" expected");
      gc();
      if (_lex[_ind].string != "{") throw std::logic_error("\"{\" expected");
      gc();
      namepace();
      if (_lex[_ind].string != "}") throw std::logic_error("\"}\" expected");
      gc();
  }

  void overload() {
      if (!type()) throw std::logic_error("type of function expected");
      gc();
      if (_lex[_ind].string != "overload") throw std::logic_error("\"overload\" expected");
      gc();
      if (_lex[_ind].type != "binary") throw std::logic_error("binary operator expected");
      gc();
      if (_lex[_ind].string != "(") throw std::logic_error("\"( \" expected");
      gc();
      parameterDef();
      if (_lex[_ind].string != ")") throw std::logic_error("\")\" expected");
      gc();
      if (_lex[_ind].string != "{") throw std::logic_error("\"{ \" expected");
      gc();
      namepace();
      if (_lex[_ind].string != "}") throw std::logic_error("\" } \" expected");
  }

  void expression() {
      if (_lex[_ind].type == "variable" || _lex[_ind].type == "number" || _lex[_ind].type == "char") {
          gc();
          if (_lex[_ind].type == "bracket" && _lex[_ind].string == "(") {
              parameters();
              if (_lex[_ind].string != ")") {
                  throw std::logic_error("expected \")\"");
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
              throw std::logic_error("\")\" expected");
          }
          gc();
          return;
      }

      if (_lex[_ind].type == "string") {
          return;
      }
      throw std::logic_error("unexpected symbols");
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
              throw std::logic_error("expected type name");
          }
          gc();
          if (_lex[_ind].type != "variable") {
              throw std::logic_error("variable expected");
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
          }
          if (_lex[_ind].string != ";") {
              throw std::logic_error("\";\" expected");
          }
          gc();
      } else {
          throw std::logic_error("unexpected symbols");
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
          globalNamespaceNoExec();
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
      if (_lex[_ind].string != "cast") throw std::logic_error("\"cat\" expected");
      gc();
      if (!type()) throw;
      gc();
      variable();
      if (_lex[_ind].string != "{") throw std::logic_error("\"{\" expected");
      gc();
      namepace();
      if (_lex[_ind].string != "}") throw std::logic_error("\"}\" expected");
      gc();
  }

  void print() {
      if (_lex[_ind].string != "(") throw std::logic_error("\"(\" expected");
      gc();
      if (_lex[_ind].type != "variable" || _lex[_ind].type != "string" ||
      _lex[_ind].type != "char" || _lex[_ind].type != "number") {
          throw std::logic_error("expression expected");
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
                  throw std::logic_error("expression expected");
              }
              gc();
          } while (_lex[_ind].string == ",");
          gc();
          if (_lex[_ind].string != ")") throw std::logic_error("\")\" expected");
      }
      gc();
  }

  void get() {
      if (_lex[_ind].string != "get") throw std::logic_error("\"get\" expected");
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
          if (_lex[_ind].string != ")") throw std::logic_error("\")\" expected");
      } else if (_lex[_ind].type == "variable") {
          gc();
          if (_lex[_ind].type != "bool") throw std::logic_error("bool operator expected");
          gc();
          if (_lex[_ind].string == "(") {
              gc();
              bool_expression();
              if (_lex[_ind].string != ")") throw std::logic_error("\")\" expected");
              gc();
          } else if (_lex[_ind].type != "variable") throw std::logic_error("variable expected");
      }
      gc();
  }

  void If() {
      if (_lex[_ind].string != "(") throw std::logic_error("\"(\" expected");
      gc();
      bool_expression();
      if (_lex[_ind].string != ")") throw std::logic_error("\")\" expected");
      gc();
      if (_lex[_ind].string != "{") throw std::logic_error("\"{\" expected");
      gc();
      namepace();
      if (_lex[_ind].string != "}") throw std::logic_error("\"}\" expected");
      gc();
      if (_lex[_ind].string != "else") return;
      gc();
      do {
          if (_lex[_ind].string == "if") {
              gc();
              if (_lex[_ind].string != "(") throw std::logic_error("\"(\" expected");
              gc();
              bool_expression();
              if (_lex[_ind].string != ")") throw std::logic_error("\")\" expected");
              gc();
          }
          if (_lex[_ind].string != "{") throw std::logic_error("\"{\" expected");
          gc();
          namepace();
          if (_lex[_ind].string != "}") throw std::logic_error("\"}\" expected");
          gc();
      } while (_lex[_ind].string == "else");
  }

  void While() {
      if (_lex[_ind].string != "(") throw std::logic_error("\"(\" expected");
      gc();
      bool_expression();
      if (_lex[_ind].string != ")") throw std::logic_error("\")\" expected");
      gc();
      if (_lex[_ind].string != "{") throw std::logic_error("\"{\" expected");
      gc();
      namepace();
      if (_lex[_ind].string != "}") throw std::logic_error("\"}\" expected");
      gc();
  }

  void dowhile() {
      if (_lex[_ind].string != "(") throw std::logic_error("\"(\" expected");
      gc();
      bool_expression();
      if (_lex[_ind].string != ")") throw std::logic_error("\")\" expected");
      gc();
      if (_lex[_ind].string != "{") throw std::logic_error("\"{\" expected");
      gc();
      namepace();
      if (_lex[_ind].string != "}") throw std::logic_error("\"}\" expected");
      gc();
  }

  void For() {
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
          bool_expression();
          if (_lex[_ind].string != ";") throw std::logic_error("\";\" expected");
          gc();
          expression();
      } else {
          throw std::logic_error("\";\" expected");
      }
      if (_lex[_ind].string != ")") throw std::logic_error("\")\" expected");
      gc();
      if (_lex[_ind].string != "{") throw std::logic_error("\"{\" expected");
      gc();
      cycle_namespace();
  }

  void variable() {
    if (_lex[_ind].type != "variable") throw std::logic_error("variable expected");
    gc();
  }

  void variable_def() {
    if (!type()) throw std::logic_error("variable name expected");
    gc();
    variable();
    if (_lex[_ind].string != "=") return;
    gc();
    expression();
  }

  void string() {
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

  void array_def() {
      if (_lex[_ind].string != "array" || _lex[_ind].type != "keywords")  {
          throw std::logic_error("\"array\" expected");
      }
      gc();
      if (!type()) throw;
      gc();
      variable();
      if (_lex[_ind].string != "(") return;
      gc();
      expression();
      if (_lex[_ind].string != ")") throw std::logic_error("\")\" expected");
      gc();
  }

  void cycle_namespace() {
      if (_lex[_ind].type == "keywords") {
          if (_lex[_ind].string == "break" || _lex[_ind].string == "continue") {
              if (_lex[_ind].string != ";") throw std::logic_error("\";\" expected");
              cycle_namespace();
          } else {
              determinantes();
              cycle_namespace();
          }
      } else {
          lexpression();
          if (_lex[_ind].string != ";") {
              throw std::logic_error("\";\" expected");
          }
          gc();
          if (_lex[_ind].string == "}") {
              return;
          }
          cycle_namespace();
          return;
      }
  }

  Lexeme GetLast() {
      return _lex[_ind];
  }

  private:
  std::vector<Lexeme> _lex;
  int _ind = 0;
};
