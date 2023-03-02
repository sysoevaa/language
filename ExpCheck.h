#include "Lexeme.h"
#include <vector>

class ExpCheck {
public:
    void Process(Lexeme lexeme);
    Lexeme GetType();
    void TailMerge();
    void Clear();
private:
    std::vector<Lexeme> _stack;
    void Push(Lexeme lexeme);
    int GetPriority();
    void Merge();
}
