#include "ArrayLiteral.hh"

void ArrayLiteral::addExpression(ptr_t<Expression> &expression) {
    expressions.push_back(std::move(expression));
}