#include "FunctionCall.hh"

std::string FunctionCall::dump(unsigned indent) const {
    std::ostringstream oss {};

    oss << Util::indent(indent) << name() << " (" << std::endl;
    oss << Util::indent(indent + 1) << "\"" << identifier << "\"" << std::endl;
    oss << argumentList->dump(indent + 1);

    oss << Util::indent(indent) << ")";

    return oss.str();
}