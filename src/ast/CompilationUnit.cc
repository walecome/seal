#include "CompilationUnit.hh"
#include "Function.hh"

void CompilationUnit::addFunction(ptr_t<Function> &function) {
    functions.push_back(std::move(function));
}

std::string CompilationUnit::dump() const { return dump(0); }

std::string CompilationUnit::dump(unsigned indent) const {
    std::ostringstream oss {};
    for (auto &x : functions) {
        oss << "Dumping function: " << std::endl;
        oss << x->dump(indent) << std::endl;
    }
    return oss.str();
}