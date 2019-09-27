#include "Operator.hh"

bool Operator::precedes(const Operator &other) const {
    return operatorPrecedence[operatorSymbol] >
           operatorPrecedence[other.operatorSymbol];
}

bool Operator::isRightAssociative() const {
    return operatorSymbol == OperatorSym::ASSIGN;
}

bool Operator::isLeftAssociative() const { return !isRightAssociative(); }