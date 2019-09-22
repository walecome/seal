#include "ParameterList.hh"

void ParameterList::addParamter(ptr_t<Parameter>& parameter) {
    parameters.push_back(std::move(parameter));
}