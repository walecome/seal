#include "ArgumentList.hh"

void ArgumentList::add_argument(ptr_t<Expression> &expression) {
    arguments.push_back(std::move(expression));
}

void ArgumentList::analyze(Scope *scope) {
    for (auto &argument : arguments) {
        argument->analyze(scope);
    }
}

std::string ArgumentList::dump(unsigned indent) const {
    std::ostringstream oss {};

    oss << util::indent(indent) << name() << " [" << std::endl;

    for (auto &x : arguments) {
        oss << x->dump(indent + 1) << std::endl;
    }

    oss << util::indent(indent) << "]" << std::endl;

    return oss.str();
}