#include "ArrayLiteral.hh"

void ArrayLiteral::add_expression(ptr_t<Expression> &expression) {
    expressions.push_back(std::move(expression));
}

void ArrayLiteral::analyze(Scope *scope) {
    type.kind = Kind::ARRAY;

    if (expressions.empty()) {
        type.primitive = Primitive::DONT_CARE;
        return;
    }

    expressions.front()->analyze(scope);
    Type first = expressions.front()->type;

    for (unsigned i = 1; i < expressions.size(); ++i) {
        auto &current = expressions.at(i);
        current->analyze(scope);
        if (current->type != first) {
            throw std::runtime_error("Mismatched types in array literal");
        }
    }

    type.primitive = first.primitive;
}

std::string ArrayLiteral::dump(unsigned indent) const {
    std::ostringstream oss {};
    oss << util::indent(indent) << name() << " [" << std::endl;

    for (auto &x : expressions) {
        oss << x->dump(indent + 1) << std::endl;
    }

    oss << util::indent(indent) << "]";

    return oss.str();
}