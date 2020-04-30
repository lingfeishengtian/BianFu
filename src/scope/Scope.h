//
// Created by Hunter Han on 2020-04-28.
//
#ifndef BIANFU_SCOPE_H
#define BIANFU_SCOPE_H

#include <iostream>
#include <map>

#include "antlr4-runtime.h"
#include "special/BFFunc.h"

class Scope{
public:
    std::string id;
    std::string type;
    Scope *parent;
    std::map<std::string, Scope*> classes;
    std::map<std::string, BFFunc> functions;
    std::map<std::string, Scope*> variables;

    Scope();
    Scope(Scope*, const std::string&);
    Scope(Scope*, const std::string&, const std::string&);

    bool isGlobal();
    std::string trace();

    virtual Scope *useOperator(const std::string&, Scope*);
    virtual Scope *clone();
    virtual std::string to_string();

    void addToGlobalRegisteredTypes(std::string type);
    void addToPrivateRegisteredTypes(std::string type);
    std::vector<std::string> getRegisteredTypes();
    Scope* getScopeFromRegisteredType(std::string type);
    BFParser::BlockContext* getFunctionBlock(std::string ID);

private:
    std::vector<std::string> registeredTypes;

};


#endif //BIANFU_SCOPE_H
