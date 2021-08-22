#include <fmt/format.h>

#include "Error.hh"
#include "FunctionDeclC.hh"
#include "Scope.hh"
#include "builtin/BuiltIn.hh"
#include "dynlib/DynLibLoader.hh"

void FunctionDeclC::analyze(Scope* scope) {
    FunctionDecl::analyze(scope);
    auto lib_or_error = dynlib::load_lib(std::string(m_lib_name), false);
    if (lib_or_error.is_error()) {
        error::add_semantic_error(
            fmt::format("Unable to load shared library \"{}\"", m_lib_name),
            source_ref);
        return;
    }
    dynlib::DynamicLibrary* lib = lib_or_error.get();
    if (!lib->has_symbol(std::string(identifier()))) {
        error::add_semantic_error(
            fmt::format("Unable to find symbol \"{}\" in shared library \"{}\"",
                        identifier(), m_lib_name));
    }
}

std::string FunctionDeclC::dump(unsigned indent) const {
    std::ostringstream oss {};

    oss << util::indent(indent) << name() << " (id=" << m_id << ") "
        << "(" << std::endl;
    oss << util::indent(indent + 1) << "C func \"" << m_identifier << "\" in \"" << m_lib_name << "\"" 
        << std::endl;
    if (m_parameter_list)
        oss << m_parameter_list->dump(indent + 1) << std::endl;
    else
        oss << util::indent(indent + 1) << "EMPTY PARAMETERLIST" << std::endl;
    oss << util::indent(indent + 1) << m_type.dump(0) << std::endl;
    oss << util::indent(indent) << ")";
    
    return oss.str();
}