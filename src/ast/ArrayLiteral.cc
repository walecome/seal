#include "ArrayLiteral.hh"

void ArrayLiteral::addExpression(ptr_t<Expression> &expression) {
    expressions.push_back(std::move(expression));
}

void ArrayLiteral::analyze(Scope *scope) const {
    for (auto &expression : expressions) {
        expression->analyze(scope);
    }
}

std::string ArrayLiteral::dump(unsigned indent) const {
    std::ostringstream oss {};
    oss << Util::indent(indent) << name() << " [" << std::endl;

    for (auto &x : expressions) {
        oss << x->dump(indent + 1) << std::endl;
    }

    oss << Util::indent(indent) << "]";

    return oss.str();
}