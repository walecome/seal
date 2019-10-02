#include "Operator.hh"

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