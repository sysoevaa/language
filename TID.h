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
  FunctionDef(std::vector<std::pair<std::string, std::string>>& _formal_parameters);
  std::string GetType();
 private:
  std::vector<std::string> _formal_parameters;
  std::string _type;
};

class StructDef : public TIDElement {
 public:
    FunctionDef* AddMethod(std::string id);
    FunctionDef* AddOperator(std::string oper, std::string type);
    void AddMember(std::string id, std::string type);
    std::string FindMember(std::string id); // returns type of member with this id
    FunctionDef* FindFunction(std::string id);
    FunctionDef* AddOverload(std::string& type, std::string& oper);
    FunctionDef* FindOverload(std::string& type, std::string& oper)
 private:
  TIDElement *_members;
  std::map<std::string, FunctionDef*> _methods;
  std::map<std::string, std::vector<FunctionDef*>> _overload;
  std::map<std::string, std::string> _operators;
};

// method for getting overloaded operator type



class TID {
 public:
  FunctionDef *AddFunction(std::string id);
  StructDef *AddStruct(std::string id);
  std::string GetType(std::string id);
  void AddGlobal(std::string id, std::string type);
  TID();
  void OpenScope();
  void CloseScope();
  std::string GetTypeOverload(std::string type1, std::string& type2, std::string& oper);
 private:
  std::map<std::string, FunctionDef *> _functions;
  std::map<std::string, StructDef *> _structs;
  std::map<std::string, std::string> _cast;
  TIDElement *_global;
  TIDElement *_current_tid;
};
