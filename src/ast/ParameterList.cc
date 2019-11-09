#include "ParameterList.hh"

void ParameterList::add_parameter(ptr_t<VariableDecl>& parameter) {
    m_parameters.push_back(std::move(parameter));
}

void ParameterList::analyze(Scope* scope) {
    for (auto& parameter : m_parameters) {
        parameter->analyze(scope);
    }
}

std::string ParameterList::dump(unsigned indent) const {
    std::ostringstream oss {};

    oss << util::indent(indent) << name() << " [" << std::endl;

    for (auto& x : m_parameters) {
        oss << x->dump(indent + 1) << std::endl;
    }

    oss << util::indent(indent) << "]";

    return oss.str();
}