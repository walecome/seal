#include "UnaryExpression.hh"

void UnaryExpression::analyze(Scope *scope) {
    expression->analyze(scope);
    op->analyze(scope);

    this->type = expression->type;
}

std::string UnaryExpression::dump(unsigned indent) const {
    std::ostringstream oss {};

    oss << util::indent(indent) << name() << " (" << std::endl;
    oss << op->dump(indent + 1) << std::endl;
    oss << expression->dump(indent + 1) << std::endl;
    oss << util::indent(indent) << ")";

    return oss.str();
}
