#include "Node.hh"
#include "Token.hh"

#include <unordered_map>

enum class OperatorSym {
    MULT,
    DIV,

    PLUS,
    MINUS,
    MODULO,

    EQ,
    NOT_EQ,
    LTEQ,
    GTEQ,
    LT,
    GT,

    ASSIGN

};

static std::unordered_map<OperatorSym, int> operatorPrecedence = {
    { OperatorSym::ASSIGN, 1 }, { OperatorSym::EQ, 5 },
    { OperatorSym::NOT_EQ, 5 }, { OperatorSym::LTEQ, 5 },
    { OperatorSym::GTEQ, 5 },   { OperatorSym::LT, 5 },
    { OperatorSym::GT, 5 },     { OperatorSym::PLUS, 10 },
    { OperatorSym::MINUS, 10 }, { OperatorSym::MULT, 20 },
    { OperatorSym::DIV, 20 }
};

static std::unordered_map<std::string, OperatorSym> stringToOpSym {
    { "*", OperatorSym::MULT },    { "/", OperatorSym::DIV },
    { "+", OperatorSym::PLUS },    { "-", OperatorSym::MINUS },
    { "%", OperatorSym::MODULO },  { "==", OperatorSym::EQ },
    { "!=", OperatorSym::NOT_EQ }, { "<=", OperatorSym::LTEQ },
    { ">=", OperatorSym::GTEQ },   { "<", OperatorSym::LT },
    { ">", OperatorSym::GT },      { "=", OperatorSym::ASSIGN },
};

static std::string fromSym(OperatorSym opSym) {
    for (auto x : stringToOpSym) {
        if (x.second == opSym) return x.first;
    }

    throw 1;
}

struct Operator : public Node {
    Operator(const Token &token)
        : operatorSymbol { stringToOpSym[token.value] },
          precedence { operatorPrecedence[operatorSymbol] } {}

    static bool isOperator(const Token &token) {
        return stringToOpSym.find(token.value) != std::end(stringToOpSym);
    }

    bool isRightAssociative() const;
    bool isLeftAssociative() const;

    virtual std::string name() const override { return "Operator"; }

    virtual std::string dump() const override {
        std::ostringstream oss {};
        // @TODO: Fix operatorSymbol to string
        oss << "[" << name() << ", " << fromSym(operatorSymbol) << "]";
        return oss.str();
    }

    bool precedes(const Operator &other) const;

    OperatorSym operatorSymbol;
    int precedence;
};