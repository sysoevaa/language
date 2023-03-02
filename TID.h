#include <vector>
#include <map>

class TIDElement {
 public:
  TIDElement();
  std::string GetType(std::string id);
  void AddVariable(std::string id, std::string type);
  TIDElement *GetParent();
  TIDElement *NewScope();

 protected:
  std::map<std::string, std::string> _id; // variable name, type
  std::vector<std::map<std::string, std::string>> _overload;
  TIDElement *parent;
};

class TID {
 public:
  TID();
  void OpenScope();
  void CloseScope();
  void AddVariable(std::string& type, std::string& name);
  std::string GetType(std::string& name);
  void AddFunction(std::string& name, std::vector<std::pair<std::string, std::string>>& formal_parameters);
  std::vector<std::string> GetParameters(std::string& func_name);
  void AddCast(std::string& type1, std::string& type2, std::string& oper);
  bool GetCast(std::string& type1, std::string& type2, std::string& oper);
  std::string GetTypeOverload(std::string& type1, std::string& type2, std::string& oper);
 private:
  std::map<std::string, TIDElement*> _functions;
  std::map<std::string, TIDElement*> _structs;
  std::map<std::string, std::vector<std::string>> _cast;
  TIDElement *_current_tid;
};
