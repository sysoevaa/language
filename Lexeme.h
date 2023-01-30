
#ifndef LANGUAGE__LEXEME_H_
#define LANGUAGE__LEXEME_H_

#include <string>
#include <vector>


struct Lexeme {
  int type;
  std::string string;
  Lexeme(int _type, std::string _string) : type(_type), string(_string) {}
};


class Analyser {
 public:
  explicit Analyser() {}

  std::vector<Lexeme> divide(std::string& string) {
      std::vector<Lexeme> divided;
      for (int i = 0; i < string.size();) {
          std::string lexeme_string;

          if (isSeparator(string[i])) {
              ++i;
              continue;
          }

          if (isLetter(string[i])) {
              lexeme_string += string[i];
              ++i;
              for (; i < string.size() && (isLetter(string[i]) || isNumber(string[i])); ++i) {
                  lexeme_string += string[i];
              }
              if (isTypeOne(lexeme_string)) {
                  divided.push_back(Lexeme(1, lexeme_string));
              }
              else {
                  divided.push_back(Lexeme(2, lexeme_string));
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
              divided.push_back(Lexeme(3, lexeme_string));
              continue;
          }

          if (i + 1 != string.size()) {
              if (isDoubleOperator(string[i], string[i + 1])) {
                  lexeme_string += string[i];
                  lexeme_string += string[i + 1];
                  i += 2;
                  divided.emplace_back(4, lexeme_string);
                  continue;
              }
          }

          if (isOperator(string[i])) {
              int j = isComment(string, i);
              if (j != -1) {
                  i = j;
                  continue;
              }
              lexeme_string += string[i];
              ++i;
              divided.push_back(Lexeme(4, lexeme_string));
              continue;
          }

          if (isPunctuation(string[i])) {
              lexeme_string += string[i];
              ++i;
              divided.push_back(Lexeme(5, lexeme_string));
              continue;
          }

          if (isBracket(string[i])) {
              lexeme_string += string[i];
              ++i;
              divided.push_back(Lexeme(6, lexeme_string));
              continue;
          }

          divided.push_back(Lexeme(-1, "ERR"));
          break;
      }
      return divided;
  }

 private:
  bool isTypeOne(std::string& string) const{
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
      if (string == "type") {
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

  bool isOperator(char symbol) const {
      switch(symbol){
          case '+':
              return true;
          case '-':
              return true;
          case '*':
              return true;
          case '/':
              return true;
          case '%':
              return true;
          case '&':
              return true;
          case '!':
              return true;
          case '~':
              return true;
          case '=':
              return true;
          case '<':
              return true;
          case '>':
              return true;
          default:
              return false;
      }
  }

  bool isDoubleOperator(char a, char b) {
      if (a == '+' && b == '+' ||
          a == '-' && b == '-' ||
          a == '&' && b == '&' ||
          a == '!' && b == '=' ||
          a == '<' && b == '=' ||
          a == '>' && b == '=' ||
          a == '|' && b == '|' ||
          a == '*' && b == '*' ) return true;
      return false;
  }

  bool isPunctuation(char symbol) const {
      switch (symbol) {
          case ';':
              return true;
          case ',':
              return true;
          case ':':
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
          ++i;
      }
      if (i + 1 == string.size()) return -1;
      return i + 2;
  }
 private:
};

#endif // LANGUAGE__LEXEME_H_
