//
// Created by А ну да on 07.05.2023.
//

#ifndef LANGUAGE_EXECUTIVE_H
#define LANGUAGE_EXECUTIVE_H

#include "PolizGenerator.h"

struct UserType {
    std::string name;

};

class Executive {
public:
    void ReadPoliz(std::string& s);

    void ExecuteProgram();
private:
    std::vector<PolizCell*> _cells;
    std::stack<int> _callStack;
    int _pos;

    void FindExec();
    void Expression();
    void AddLocalVariable();
    void AddGlobalVariable();
    void Jump();
    void OperatorJump();
};


#endif //LANGUAGE_EXECUTIVE_H
