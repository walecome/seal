#include "ParameterList.hh"

ParameterList::~ParameterList() {
    for (Parameter* parameter : parameters) {
        delete parameter;
    }
}

void ParameterList::addParamter(Parameter* parameter) {
    parameters.push_back(parameter);
}