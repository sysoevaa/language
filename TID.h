#pragma once
#include <vector>
#include <map>
#include <string>

class TIDElement {
 public:
  TIDElement();
  void SetType(std::string& id);
  std::string GetType(std::string id, bool func = false);
  void AddVariable(std::string id, std::string type);
  TIDElement *GetParent();
  void SetParent(TIDElement* parent);
  std::map<std::string, std::string> GetId();
  TIDElement* AddOverload(std::string& type, std::string& oper, std::string& ret);
  std::string GetOverloadType(std::string& type, std::string& oper);

 protected:
  std::map<std::string, std::string> _id; // variable name, type
  std::map<std::pair<std::string, std::string>, TIDElement*> _overload;
  std::string _type;
  TIDElement *_parent;
};

class TID {
 public:
  TID();
  void OpenScope();
  void CloseScope();
  void AddVariable(std::string& type, std::string& name);
  std::string GetType(std::string& name);
  void AddFunction(std::string& name, std::vector<std::pair<std::string,
                   std::string>>& formal_parameters, std::string& return_type);
  std::vector<std::string> GetParameters(std::string& func_name);
  void AddCast(std::string& type1, std::string& type2);
  bool GetCast(std::string& type1, std::string& type2);
  void AddOverload(std::string& type, std::string& oper, std::string& ret);
  void AddStruct(std::string& type);
  std::string GetTypeOverload(std::string& type1, std::string& type2, std::string& oper);
  std::string GetMember(std::string& type, std::string& name);
  std::string GetTypeFunction(std::string& name);
  bool IsTypeExist(std::string& type);
  std::string GetCurrentReturnType();
 private:
  std::map<std::string, TIDElement*> _functions;
  std::map<std::string, TIDElement*> _structs;
  std::vector<std::pair<std::pair<std::string, std::string>, TIDElement*>> _cast;
  TIDElement *_current_tid;
};
