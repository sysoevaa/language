#include "Lexeme.h"
#include <vector>

class ExpCheck {
public:
    void Process(Lexeme lexeme);
    void Push(Lexeme lexeme);
    int GetPriority();
    void Merge();
    Lexeme GetType();
private:
    std::vector<Lexeme> _stack;
}
