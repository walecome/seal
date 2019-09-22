#include "Node.hh"
#include "Token.hh"

#include <unordered_map>

static std::unordered_map<Token, int> operatorPrecedence = {
    { ASSIGN, 1 }, { EQ, 5 },    { NOT_EQ, 5 }, { LTEQ, 5 },
    { GTEQ, 5 },   { LT, 5 },    { GT, 5 },     { PLUS, 10 },
    { MINUS, 10 }, { STAR, 20 }, { SLASH, 20 }
};

struct Operator : public Node {
    static bool isOperator(const Token &token) {
        operatorPrecedence.find(token) != std::end(operatorPrecedence);
    };
};