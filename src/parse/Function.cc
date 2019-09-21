#include "Function.hh"

Function::~Function() {
    if (functionDecl) delete functionDecl;
    if (block) delete block;
}