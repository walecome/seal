#include "ArgumentList.hh"

void ArgumentList::addArgument(ptr_t<Expression> &expression) {
    arguments.push_back(std::move(expression));
}