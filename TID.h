#include <vector>
#include <map>

class TIDElement {
 public:
  std::string GetType(std::string id);
  void AddVariable(std::string id, std::string type);
  TIDElement *GetParent();
  TIDElement *NewScope();
 protected:
  std::map<std::string, std::string> _id; // variable name, type
  std::vector<TIDElement *> _scopes;
  TIDElement *parent;
};

class FunctionDef : TIDElement {
 public:
  std::vector<std::string> GetParameters();
  FunctionDef();
 private:
  std::vector<std::string> _formal_parameters;
};

class StructDef : TIDElement {
 public:
    FunctionDef* AddMethod(std::string id);
    void AddMember();
    TIDElement* FindMember(std::string id);
    FunctionDef* FindFunction(std::string id);
 private:
  TIDElement *_members;
  FunctionDef *_methods;
};

class TID {
 public:
  FunctionDef *AddFunction(std::string id);
  StructDef *AddStruct(std::string id);
 private:
  std::map<std::string, FunctionDef *> _functions;
  std::map<std::string, StructDef *> _structs;
  TIDElement *global;
  TIDElement *_current_tid;
};