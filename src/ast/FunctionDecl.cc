#include <fmt/format.h>

#include "FunctionDecl.hh"
#include "builtin/BuiltIn.hh"

int new_function_id() {
    static int function_id = BuiltIn::number_of_builtins();
    return function_id++;
}

std::string FunctionDecl::dump(unsigned indent) const {
    std::ostringstream oss {};

    oss << util::indent(indent) << name() << " (id=" << m_id << ") "
        << "(" << std::endl;
    oss << util::indent(indent + 1) << "\"" << m_identifier << "\""
        << std::endl;
    if (m_parameter_list)
        oss << m_parameter_list->dump(indent + 1) << std::endl;
    else
        oss << util::indent(indent + 1) << "EMPTY PARAMETERLIST" << std::endl;
    oss << util::indent(indent + 1) << m_type.dump(0) << std::endl;
    oss << util::indent(indent) << ")";

    return oss.str();
}