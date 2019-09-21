#include "FunctionDecl.hh"

FunctionDecl::~FunctionDecl() {
    if (parameterList) delete parameterList;
}