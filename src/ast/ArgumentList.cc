#include "ArgumentList.hh"

void ArgumentList::addArgument(ptr_t<Expression> &expression) {
    arguments.push_back(std::move(expression));
}

void ArgumentList::analyze(Scope *scope) const {
    for (auto &argument : arguments) {
        argument->analyze(scope);
    }
}

std::string ArgumentList::dump(unsigned indent) const {
    std::ostringstream oss {};

    oss << Util::indent(indent) << name() << " [" << std::endl;

    for (auto &x : arguments) {
        oss << x->dump(indent + 1) << std::endl;
    }

    oss << Util::indent(indent) << "]" << std::endl;

    return oss.str();
}