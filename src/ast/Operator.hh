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

    ASSIGN,

    AND,
    OR,
};

static const std::unordered_map<std::string_view, OperatorSym>
    string_to_op_sym {
        { "*", OperatorSym::MULT },    { "/", OperatorSym::DIV },
        { "+", OperatorSym::PLUS },    { "-", OperatorSym::MINUS },
        { "%", OperatorSym::MODULO },  { "==", OperatorSym::EQ },
        { "!=", OperatorSym::NOT_EQ }, { "<=", OperatorSym::LTEQ },
        { ">=", OperatorSym::GTEQ },   { "<", OperatorSym::LT },
        { ">", OperatorSym::GT },      { "=", OperatorSym::ASSIGN },
        { "++", OperatorSym::INC },    { "--", OperatorSym::DEC },
        { "&&", OperatorSym::AND },    { "||", OperatorSym::OR }
    };

class Operator : public Node {
    MAKE_DEFAULT_CONSTRUCTABLE(Operator)

    Operator(const Token &token)
        : m_operator_symbol { string_to_op_sym.at(token.value) } {}

    static bool is_operator(const Token &token) {
        return string_to_op_sym.find(token.value) != std::end(string_to_op_sym);
    }

    virtual void analyze(Scope *) override {}
    virtual std::string name() const override { return "Operator"; }
    virtual std::string dump(unsigned indent) const override;

    OperatorSym symbol() const { return m_operator_symbol; }

   private:
    OperatorSym m_operator_symbol;
};