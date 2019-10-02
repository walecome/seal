#include "FunctionDecl.hh"

void FunctionDecl::analyze(Scope *scope) const {
    // @TODO: We don't check the function identifier since this is done in the
    //        initial sematic pass. But this creates an issue with not
    //        function inside other functions.
    parameterList->analyze(scope);
}

std::string FunctionDecl::dump(unsigned indent) const {
    std::ostringstream oss {};

    oss << Util::indent(indent) << name() << "(" << std::endl;
    oss << Util::indent(indent + 1) << "\"" << identifier << "\"" << std::endl;
    if (parameterList)
        oss << parameterList->dump(indent + 1) << std::endl;
    else
        oss << Util::indent(indent + 1) << "EMPTY PARAMETERLIST" << std::endl;
    oss << Util::indent(indent) << ")";

    return oss.str();
}