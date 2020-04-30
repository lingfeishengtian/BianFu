//
// Created by Hunter Han on 2020-04-28.
//

#include "Scope.h"
#include "../error/BianFuError.h"

Scope::Scope() {
    parent = nullptr;
    id = std::string();
}

Scope::Scope(Scope *scope, const std::string &iD) {
    parent = scope;
    id = iD;
}

Scope::Scope(Scope *scope, const std::string &iD, const std::string &t) {
    parent = scope;
    id = iD;
    type = t;

    addToGlobalRegisteredTypes(type);
}

bool Scope::isGlobal() {
    return parent == nullptr;
}

std::string Scope::trace() {
    if(!isGlobal())
        return id + "->" + parent->trace();
    else
        return id;
}

Scope* Scope::useOperator(const std::string&, Scope *) {
    return nullptr;
}

std::string Scope::to_string() {
    return id;
}

void Scope::addToGlobalRegisteredTypes(std::string type) {
    if(std::count(registeredTypes.begin(), registeredTypes.end(), type)) throw BianFuError("不能找到来历", type + "名字已经被站了。");
    if(parent != nullptr) addToGlobalRegisteredTypes(type);
    else registeredTypes.push_back(type);
}

void Scope::addToPrivateRegisteredTypes(std::string type) {
    registeredTypes.push_back(type);
}

std::vector<std::string> Scope::getRegisteredTypes() {
    if(parent == nullptr) return (registeredTypes);
    else{
        std::vector<std::string> tmp;
        std::vector<std::string> reg = parent->getRegisteredTypes();
        tmp.insert(tmp.end(), registeredTypes.begin(), registeredTypes.end());
        tmp.insert(tmp.end(), reg.begin(), reg.end());
        return tmp;
    }
}

Scope *Scope::getScopeFromRegisteredType(std::string type) {
    if(std::count(registeredTypes.begin(), registeredTypes.end(), type)){
        return classes[type];
    }else if(parent != nullptr){
        return parent->getScopeFromRegisteredType(type);
    }
    return nullptr;
}

BFParser::BlockContext *Scope::getFunctionBlock(std::string ID) {
    if(functions.find(ID) != functions.end()){
        return functions[ID].blockContext;
    }else if(parent != nullptr){
        return getFunctionBlock(ID);
    }else{
        throw BianFuError("不能找到孩子。", "没有" + ID + "的功能。");
    }
}

Scope *Scope::clone() {
    Scope* scope = new Scope(parent, id);
    scope->type = type;
    scope->classes = classes;
    scope->functions = functions;
    scope->registeredTypes = registeredTypes;
    scope->variables = variables;
    return scope;
}
