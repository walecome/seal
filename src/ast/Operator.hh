#pragma once

#include <unordered_map>
#include <iterator>

#include "Node.hh"
#include "Token.hh"
#include "Type.hh"

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

namespace {
    static std::unordered_map<OperatorSym, std::string_view> reverse(const std::unordered_map<std::string_view, OperatorSym> map) {
        std::unordered_map<OperatorSym, std::string_view> result {};
        for (auto it = map.begin(); it != map.end(); it = std::next(it)) {
            result.insert({ it->second, it->first });
        }
        return result;
    }
}

static const std::unordered_map<OperatorSym, std::string_view> op_sym_to_string = reverse(string_to_op_sym);

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

    std::string_view to_operator_string() const {
        return op_sym_to_string.at(m_operator_symbol);
    }

    OperatorSym symbol() const { return m_operator_symbol; }

    bool accepts(Type type) const;

   private:
    OperatorSym m_operator_symbol;
};
