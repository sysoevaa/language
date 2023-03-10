#include "Lexeme.h"
#include <vector>
#include "TID.h"

class ExpCheck {
public:
    ExpCheck(TID* tid) : _tid(tid) { }
    void Process(Lexeme lexeme);
    Lexeme GetType();
    void TailMerge();
    void Clear();
private:
    std::vector<Lexeme> _stack;
    TID* _tid;
    void Push(Lexeme lexeme);
    int GetPriority();
    void Merge();
};
