
#ifndef LANGUAGE__LEXEME_H_
#define LANGUAGE__LEXEME_H_

#include <string>
#include <vector>


struct Lexeme {
  std::string string, type;
  int num;
  int priority;
  Lexeme(const std::string& _type, const std::string& _string, int _num, int _priority = 0) :
  type(_type), string(_string), num(_num), priority(_priority) {}
};


class Analyser {
 public:
  explicit Analyser() {}

  std::vector<Lexeme> divide(std::string& string) {
      std::vector<Lexeme> divided;
      for (int i = 0; i < string.size();) {
          std::string lexeme_string;

          if (isSeparator(string[i])) {
              if (string[i] == '\n') ++num;
              ++i;
              continue;
          }

          if (isLetter(string[i])) {
              int type = -1;
              if (divided.size() > 0 && divided.back().string == "\'") {
                  type = 1;
              } else if (divided.size() > 0 && divided.back().string == "\"") {
                  type = 2;
              }
              // lexeme_string += string[i];
              // ++i;
              if (type == 1 || type == 2) {
                  while (i < string.size() && (string[i] != '\"' && string[i] != '\'')) {
                      lexeme_string += string[i];
                      ++i;
                  }
                  if (i == string.size()) divided.emplace_back("ERR", "Err", -1);
              }
              for (; i < string.size() && (isLetter(string[i]) || isNumber(string[i]) || string[i] == '_'); ++i) {
                  lexeme_string += string[i];
              }
              if (type == 1) {
                  divided.pop_back();
                  lexeme_string.push_back(' ');
                  divided.emplace_back("char", lexeme_string, num);
                  ++i;
              } else if (type == 2) {
                  divided.pop_back();
                  lexeme_string.push_back(' ');
                  divided.emplace_back("string", lexeme_string, num);
                  ++i;
              } else {
                  if (isTypeOne(lexeme_string)) {
                      divided.push_back(Lexeme("keyword", lexeme_string, num));
                  } else {
                      divided.push_back(Lexeme("variable", lexeme_string, num)); // variable
                  }
              }
              continue;
          }

          if (isNumber(string[i])) {
              lexeme_string += string[i];
              ++i;
              for (;i < string.size() && isNumber(string[i]); ++i) {
                  lexeme_string += string[i];
              }
              if (i < string.size() && string[i] == '.') {
                  lexeme_string += string[i];
                  ++i;
                  for (;i < string.size() && isNumber(string[i]); ++i) {
                      lexeme_string += string[i];
                  }
              }
              divided.push_back(Lexeme("number", lexeme_string, num));
              continue;
          }

          if (i + 1 != string.size()) {
              if (isDoubleOperator(string[i], string[i + 1]) == 1) {
                  lexeme_string += string[i];
                  lexeme_string += string[i + 1];
                  i += 2;
                  divided.emplace_back("unary", lexeme_string, num, 1);
                  continue;
              } else if (isDoubleOperator(string[i], string[i + 1]) == 2) {
                  lexeme_string += string[i];
                  lexeme_string += string[i + 1];
                  i += 2;
                  if (string[i] == '&') {
                      divided.emplace_back("bool", lexeme_string, num, 6);
                  }
                  if (string[i] == '|') {
                      divided.emplace_back("bool", lexeme_string, num, 7);
                  }

                  continue;
              } else if (isDoubleOperator(string[i], string[i + 1]) == 3) {
                  lexeme_string += string[i];
                  lexeme_string += string[i + 1];
                  i += 2;
                  divided.emplace_back("power", lexeme_string, num, 1);
                  continue;
              }
          }

          if (isOperator(string[i])) {
              int j = isComment(string, i);
              if (j != -1) {
                  i = j;
                  continue;
              }
              int priority = isOperator(string[i]);
              lexeme_string += string[i];
              if (string[i] == '=') {
                  divided.emplace_back("equals", lexeme_string, num, 8);
              }
              else if (string[i] == '!' || string[i] == '-' && i != 0 && (divided.back().type != "variable" ||
              divided.back().type != "string" && divided.back().type != "number")) {
                  divided.emplace_back("unary", lexeme_string, num, 1);
              }
              else if (string[i] == '<' || string[i] == '>') {
                  divided.emplace_back("bool", lexeme_string, num, priority);
              } else {
                  divided.push_back(Lexeme("binary", lexeme_string, num, priority));
              }
              ++i;
              continue;
          }

          if (isPunctuation(string[i])) {
              lexeme_string += string[i];
              ++i;
              divided.push_back(Lexeme("punct", lexeme_string, num));
              if (divided.back().string == ".") {
                  divided.back().priority = 0;
              }
              continue;
          }
          if (isBracket(string[i])) {
              lexeme_string += string[i];
              ++i;
              divided.push_back(Lexeme("bracket", lexeme_string, num));
              continue;
          }

          divided.push_back(Lexeme("err", "ERR", -1));
          break;
      }
      return divided;
  }

 private:
  bool isTypeOne(std::string& string) const{
      if (string == "void") {
          return true;
      }
      if (string == "for") {
          return true;
      }
      if (string == "while") {
          return true;
      }
      if (string == "char") {
          return true;
      }
      if (string == "int32") {
          return true;
      }
      if (string == "bool") {
          return true;
      }
      if (string == "int64") {
          return true;
      }
      if (string == "float32") {
          return true;
      }
      if (string == "float64") {
          return true;
      }
      if (string == "string") {
          return true;
      }
      if (string == "if") {
          return true;
      }
      if (string == "else") {
          return true;
      }
      if (string == "exec") {
          return true;
      }
      if (string == "do") {
          return true;
      }
      if (string == "cast") {
          return true;
      }
      if (string == "return") {
          return true;
      }
      if (string == "get") {
          return true;
      }
      if (string == "print") {
          return true;
      }
      if (string == "struct") {
          return true;
      }
      if (string == "construct") {
          return true;
      }
      if (string == "overload") {
          return true;
      }
      if (string == "break") {
          return true;
      }
      if (string == "continue") {
          return true;
      }


      return false;
  }

  bool isLetter(char symbol) const {
      return symbol >= 'A' && symbol <= 'Z' || symbol >= 'a' && symbol <= 'z';
  }

  bool isNumber(char symbol) const {
      return symbol >= '0' && symbol <= '9';
  }

  bool isSeparator(char symbol) const {
      return symbol == ' ' || symbol == '\n';
  }

  int isOperator(char symbol) const {
      switch(symbol){
          case '+':
              return 3;
          case '-':
              return 3;
          case '*':
              return 2;
          case '/':
              return 2;
          case '%':
              return 2;
          case '&':
              return 2;
          case '!':
              return 2;
          case '~':
              return 2;
          case '=':
              return 8;
          case '<':
              return 4;
          case '>':
              return 4;
          default:
              return 0;
      }
  }

  int isDoubleOperator(char a, char b) {
      if (a == '+' && b == '+' ||
          a == '-' && b == '-') return 1;
      if (a == '&' && b == '&' ||
          a == '!' && b == '=' ||
          a == '<' && b == '=' ||
          a == '>' && b == '=' ||
          a == '|' && b == '|' ) return 2;
      if (a == '*' && b == '*' ) return 3;
      return 0;
  }

  bool isPunctuation(char symbol) const {
      switch (symbol) {
          case ';':
              return true;
          case ',':
              return true;
          case ':':
              return true;
          case '.':
              return true;
          default:
              return false;
      }
  }

  bool isBracket(char symbol) const {
      switch (symbol) {
          case '(':
              return true;
          case ')':
              return true;
          case '{':
              return true;
          case '}':
              return true;
          case '[':
              return true;
          case ']':
              return true;
          case '\'':
              return true;
          case '\"':
              return true;
          default:
              return false;
      }
  }
  int isComment(std::string& string, int i) {
      if (string[i] != '/' || i + 1 == string.size()) return -1;
      if (string[i + 1] != '*') return -1;
      i += 2;
      while (i + 1 < string.size()) {
          if (string[i] == '*' && string[i + 1] == '/') break;
          if (string[i] == '\n') ++num;
          ++i;
      }
      if (i + 1 == string.size()) return -1;
      return i + 2;
  }
 private:
  int num = 1;
};

#endif // LANGUAGE__LEXEME_H_
