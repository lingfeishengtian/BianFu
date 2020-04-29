//
// Created by Hunter Han on 2020-04-28.
//
#ifndef BIANFU_SCOPE_H
#define BIANFU_SCOPE_H

#include <iostream>
#include <map>

#include "antlr4-runtime.h"

class Scope{
public:
    std::string id;
    Scope *parent;
    std::map<std::string, Scope*> classes;
    std::map<std::string, Scope*> functions;
    std::map<std::string, Scope*> variables;

    Scope();
    Scope(Scope*, const std::string&);

    bool isGlobal();
    std::string trace();

    virtual Scope *useOperator(const std::string&, Scope*);
};


#endif //BIANFU_SCOPE_H
