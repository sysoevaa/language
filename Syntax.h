#include "Lexeme.h"
#include "ExpCheck.h"
#include "TID.h"
#include <iostream>
#include "PolizGenerator.h"


class SyntaxAnalyser {
 public:
  SyntaxAnalyser (const std::vector<Lexeme>& lex) : _lex(lex), _tid(new TID), expCheck(_tid), _list(new DefinitionList) {
      _gen = new PolizGenerator(_tid, _list);
  }
  void gc();

  bool type();

  void program();

  void globalNamespaceNoExec();

  void globalNamespace();

  void object();

  void member();

  void construct();

  void overload();

  void expression();

  void parameters();

  void parameterDef();

  void determinantes();

  void lexpression();

  void namepace();

  void functionDefinition();

  void type_cast();

  void type_cast_def();

  void print();

  void get();

  void input();

  void If();

  void While();

  void dowhile();

  void For();

  void variable();

  void variable_def();

  void string();

  void array_def();

  void cycle_namespace();

  std::string GetArrayType(std::string& s);

  std::string IsEqualTypes(std::string& type1, std::string& type2);

  Lexeme GetLast() {
      if (_lex[_ind].string == "END") return _lex[_ind];
      if (_ind == 0) return _lex[_ind];
      return _lex[_ind - 1];
  }
  TID* _tid;
 private:
  std::vector<Lexeme> _lex;
  int _ind = 0;
  ExpCheck expCheck;
  std::vector<std::pair<std::string, std::string>> _parameter_def_arr;
  std::vector<std::string> _parameter_arr;
  std::string _save_type = "null";
  std::string object_name;
  bool is_method = false;
  bool is_in_function = false;
  bool IAMRETURNINGSOMETHING = false;
  std::string method_return_type = "null";
  std::string method_name = "null";
  int _begin_index, _end_index;
  PolizGenerator* _gen;
  DefinitionList* _list;
};