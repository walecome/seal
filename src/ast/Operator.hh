#pragma once

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

static std::unordered_map<OperatorSym, int> operator_precedence = {
    { OperatorSym::ASSIGN, 1 }, { OperatorSym::EQ, 5 },
    { OperatorSym::NOT_EQ, 5 }, { OperatorSym::LTEQ, 5 },
    { OperatorSym::GTEQ, 5 },   { OperatorSym::LT, 5 },
    { OperatorSym::GT, 5 },     { OperatorSym::PLUS, 10 },
    { OperatorSym::MINUS, 10 }, { OperatorSym::MULT, 20 },
    { OperatorSym::DIV, 20 }
};

static std::unordered_map<std::string_view, OperatorSym> string_to_op_sym {
    { "*", OperatorSym::MULT },    { "/", OperatorSym::DIV },
    { "+", OperatorSym::PLUS },    { "-", OperatorSym::MINUS },
    { "%", OperatorSym::MODULO },  { "==", OperatorSym::EQ },
    { "!=", OperatorSym::NOT_EQ }, { "<=", OperatorSym::LTEQ },
    { ">=", OperatorSym::GTEQ },   { "<", OperatorSym::LT },
    { ">", OperatorSym::GT },      { "=", OperatorSym::ASSIGN },
};

struct Operator : public Node {
    Operator(const Token &token)
        : operator_symbol { string_to_op_sym[token.value] },
          precedence { operator_precedence[operator_symbol] } {}

    static bool is_operator(const Token &token) {
        return string_to_op_sym.find(token.value) != std::end(string_to_op_sym);
    }

    bool is_right_associative() const;
    bool is_left_associative() const;

    virtual std::string name() const override { return "Operator"; }

    virtual std::string dump(unsigned indent) const override;

    bool precedes(const Operator &other) const;

    OperatorSym operator_symbol;
    int precedence;
};