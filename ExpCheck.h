#include "Lexeme.h"
#include <vector>
#include "TID.h"

class ExpCheck {
public:
    ExpCheck(TID* tid) : _tid(tid) { }
    void Process(Lexeme lexeme);
    Lexeme GetType();
    Lexeme GetLast();
    void ChangeLast(std::string s);
    void TailMerge();
    void Clear();
    std::vector<Lexeme> GetStack();
    void SetStack(std::vector<Lexeme>& stack);

private:
    std::vector<Lexeme> _stack;
    TID* _tid;
    void Push(Lexeme lexeme);
    int GetPriority();
    void Merge();
};
