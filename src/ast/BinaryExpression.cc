#include "BinaryExpression.hh"

void BinaryExpression::analyze(Scope *scope) const {
    left->analyze(scope);
    op->analyze(scope);
    right->analyze(scope);
}

std::string BinaryExpression::dump(unsigned indent) const {
    std::ostringstream oss {};

    oss << Util::indent(indent) << name() << " (" << std::endl;
    oss << left->dump(indent + 1) << std::endl;
    oss << op->dump(indent + 1) << std::endl;
    oss << right->dump(indent + 1) << std::endl;
    oss << Util::indent(indent) << ")";

    return oss.str();
}