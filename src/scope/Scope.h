//
// Created by Hunter Han on 2020-04-28.
//
#include <iostream>
#include <map>
#include "BFVariable.h"

#ifndef BIANFU_SCOPE_H
#define BIANFU_SCOPE_H

class Scope {
public:
    Scope* parent;
    std::map<std::string, Scope> classes;
    std::map<std::string, Scope> functions;
    std::map<std::string, BFVariable> variables;

    Scope();
    Scope(Scope* scope);
    bool isGlobal();
};


#endif //BIANFU_SCOPE_H
