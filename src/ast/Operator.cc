#include "Operator.hh"

static std::string_view from_sym(OperatorSym op_sym) {
    for (auto x : string_to_op_sym) {
        if (x.second == op_sym) return x.first;
    }

    throw 1;
}

std::string Operator::dump(unsigned indent) const {
    std::ostringstream oss {};
    oss << util::indent(indent) << "{" << name() << ", "
        << from_sym(m_operator_symbol) << "}";
    return oss.str();
}

bool Operator::accepts(Type type) const {

    if (type.is_string()) {
        return symbol() == OperatorSym::PLUS || symbol() == OperatorSym::ASSIGN;
    }

    // TODO: We just assume that all other types accepts all operators, they probably shouldn't.
    return true;
}
