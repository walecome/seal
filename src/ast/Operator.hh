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

    INC,
    DEC,

    EQ,
    NOT_EQ,
    LTEQ,
    GTEQ,
    LT,
    GT,

    ASSIGN

};

static std::unordered_map<std::string_view, OperatorSym> string_to_op_sym {
    { "*", OperatorSym::MULT },    { "/", OperatorSym::DIV },
    { "+", OperatorSym::PLUS },    { "-", OperatorSym::MINUS },
    { "%", OperatorSym::MODULO },  { "==", OperatorSym::EQ },
    { "!=", OperatorSym::NOT_EQ }, { "<=", OperatorSym::LTEQ },
    { ">=", OperatorSym::GTEQ },   { "<", OperatorSym::LT },
    { ">", OperatorSym::GT },      { "=", OperatorSym::ASSIGN },
    { "++", OperatorSym::INC },    { "--", OperatorSym::DEC }
};

class Operator : public Node {
    MAKE_DEFAULT_CONSTRUCTABLE(Operator)

    Operator(const Token &token)
        : token { token }, operator_symbol { string_to_op_sym[token.value] } {}

    static bool is_operator(const Token &token) {
        return string_to_op_sym.find(token.value) != std::end(string_to_op_sym);
    }

    bool is_right_associative() const;
    bool is_left_associative() const;

    virtual void analyze(Scope *) override {}

    virtual std::string name() const override { return "Operator"; }

    virtual std::string dump(unsigned indent) const override;

    const Token token;
    OperatorSym operator_symbol;
};