#include "ParameterList.hh"

void ParameterList::addParamter(ptr_t<Parameter>& parameter) {
    parameters.push_back(std::move(parameter));
}

std::string ParameterList::dump(unsigned indent) const {
    std::ostringstream oss {};

    oss << Util::indent(indent) << name() << " [" << std::endl;

    for (auto& x : parameters) {
        oss << x->dump(indent + 1) << std::endl;
    }

    oss << Util::indent(indent) << "]";

    return oss.str();
}