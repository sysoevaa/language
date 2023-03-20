#include "Lexeme.h"
#include "ExpCheck.h"
#include "TID.h"


class SyntaxAnalyser {
 public:
  SyntaxAnalyser (const std::vector<Lexeme>& lex) : _lex(lex), _tid(new TID), expCheck(_tid) {}

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
      return _lex[_ind];
  }
  TID* _tid;
 private:
  std::vector<Lexeme> _lex;
  int _ind = 0;
  ExpCheck expCheck;
  std::vector<std::pair<std::string, std::string>> _parameter_def_arr;
  std::vector<std::string> _parameter_arr;
  std::string _save_type = "null";
};