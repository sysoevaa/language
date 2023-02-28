#include <vector>
#include <map>

class TIDElement {
 public:
  std::string GetType(std::string id);
  std::string AddVariable(std::string id, std::string type);
  TIDElement *GetParent();
  void NewScope();
 private:
  std::map<std::string, std::string> _id; // variable name, type
  std::vector<TIDElement *> _scopes;
};

class FunctionDef : public TIDElement {
 private:
  TIDElement *_formal_parameters;

};

class TID {
 public:

 private:
  std::map<std::string, FunctionDef *> _functions;
};