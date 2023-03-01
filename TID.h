#include <vector>
#include <map>

class TIDElement {
 public:
  std::string GetType(std::string id);
  void AddVariable(std::string id, std::string type);
  TIDElement *GetParent();
  TIDElement *NewScope();
  TIDElement();
 protected:
  std::map<std::string, std::string> _id; // variable name, type
  std::vector<TIDElement *> _scopes;
  TIDElement *parent;
};

class FunctionDef : public TIDElement {
 public:
  std::vector<std::string> GetParameters();
  FunctionDef();
  std::string GetType();
 private:
  std::vector<std::string> _formal_parameters;
  std::string _type;
};

class StructDef : public TIDElement {
 public:
    FunctionDef* AddMethod(std::string id);
    void AddMember(std::string id, std::string type);
    std::string FindMember(std::string id); // returns type of member with this id
    FunctionDef* FindFunction(std::string id);
 private:
  TIDElement *_members;
  std::map<std::string, FunctionDef*> _methods;
  std::map<std::string, std::string> _operators;
};

class TID {
 public:
  FunctionDef *AddFunction(std::string id);
  StructDef *AddStruct(std::string id);
  std::string GetType(std::string id);
  void AddGlobal();
  TID();
  void OpenScope();
  void CloseScope();
 private:
  std::map<std::string, FunctionDef *> _functions;
  std::map<std::string, StructDef *> _structs;
  TIDElement *_global;
  TIDElement *_current_tid;
};