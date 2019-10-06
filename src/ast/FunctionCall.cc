#include "FunctionCall.hh"

void FunctionCall::analyze(Scope *scope) const {
    if (!scope->hasFunction(identifier)) {
        // @TODO: Implement semantic error instead
        throw std::runtime_error("Call to undeclared function " +
                                 std::string(identifier));
    }

    argumentList->analyze(scope);
}

std::string FunctionCall::dump(unsigned indent) const {
    std::ostringstream oss {};

    oss << Util::indent(indent) << name() << " (" << std::endl;
    oss << Util::indent(indent + 1) << "\"" << identifier << "\"" << std::endl;
    oss << argumentList->dump(indent + 1);

    oss << Util::indent(indent) << ")";

    return oss.str();
}