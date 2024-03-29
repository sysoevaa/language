#pragma once
#include <vector>
#include <map>
#include <string>

class TIDElement {
 public:
  TIDElement();
  void SetType(std::string& id);
  std::string GetType(std::string id, bool func = false);
  void AddVariable(std::string& id, std::string& type);
  TIDElement *GetParent();
  void SetParent(TIDElement* parent);
  std::map<std::string, std::string> GetId();
  TIDElement* AddOverload(std::string& type, std::string& oper, std::string& ret);
  std::string GetOverloadType(std::string& type, std::string& oper);
  TIDElement* AddConstructor(std::vector<std::pair<std::string, std::string>>& id);
  TIDElement* AddMethod(std::vector<std::pair<std::string, std::string>>& id, std::string& name, std::string& ret);
  std::vector<std::string> GetMethodParam(std::string& method_name);
  std::vector<std::string> GetConstructorParam();
  std::string GetMethodType(std::string& method_name);
  std::map<std::string, TIDElement*> GetMethods();
 protected:
  std::map<std::string, std::string> _id; // variable name, type
  std::map<std::pair<std::string, std::string>, TIDElement*> _overload;
  std::map<std::string, TIDElement*> _methods;
  std::string _type;
  TIDElement *_parent;
  TIDElement* _construct;
};

class TID {
 public:
  TID();
  void OpenScope();
  void CloseScope();
  void AddVariable(std::string& type, std::string& name);
  std::string GetType(const std::string& name);
  void AddFunction(std::string& name, std::vector<std::pair<std::string,
                   std::string>>& formal_parameters, std::string& return_type);
  std::vector<std::string> GetParameters(std::string& func_name);
  void AddCast(std::string& type1, std::string& type2, std::string& id);
  bool GetCast(std::string& type1, std::string& type2);
  void AddOverload(std::string& id, std::string& type, std::string& oper, std::string& ret);
  void AddStruct(std::string& type);
  void AddConstructor(std::vector<std::pair<std::string, std::string>>& id);
  std::string GetTypeOverload(std::string& type1, std::string& type2, std::string& oper);
  std::string GetMember(std::string& type, std::string& name);
  std::string GetTypeFunction(std::string& name);
  int IsTypeExist(std::string type);
  std::string GetCurrentReturnType();
  bool IsFunctionExist(std::string& id);
  void AddMethod(std::vector<std::pair<std::string, std::string>>& id, std::string& name, std::string& ret);
  std::vector<std::string> GetMethodParameters(std::string& struct_name, std::string& method_name);
  std::string GetMethodType(std::string& struct_name, std::string& method_name);
  std::vector<std::string> GetConstructorParameters(std::string& struct_name);
  TIDElement* GetGlobal();
  std::map<std::string, TIDElement*> GetFunc();
  std::vector<std::pair<std::pair<std::string, std::string>, TIDElement*>> GetCast();
  std::map<std::string, TIDElement*> GetStructs();

private:
  std::map<std::string, TIDElement*> _functions;
  std::map<std::string, TIDElement*> _structs;
  std::vector<std::pair<std::pair<std::string, std::string>, TIDElement*>> _cast;
  TIDElement *_current_tid, *_global_tid;
};
