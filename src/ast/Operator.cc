#include "Operator.hh"

static std::string_view fromSym(OperatorSym opSym) {
    for (auto x : stringToOpSym) {
        if (x.second == opSym) return x.first;
    }

    throw 1;
}

bool Operator::precedes(const Operator &other) const {
    return operatorPrecedence[operatorSymbol] >
           operatorPrecedence[other.operatorSymbol];
}

bool Operator::isRightAssociative() const {
    return operatorSymbol == OperatorSym::ASSIGN;
}

bool Operator::isLeftAssociative() const { return !isRightAssociative(); }

std::string Operator::dump(unsigned indent) const {
    std::ostringstream oss {};
    oss << Util::indent(indent) << "{" << name() << ", "
        << fromSym(operatorSymbol) << "}";
    return oss.str();
}