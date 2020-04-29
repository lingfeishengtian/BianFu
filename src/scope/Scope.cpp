//
// Created by Hunter Han on 2020-04-28.
//

#include "Scope.h"

Scope::Scope() {
    parent = nullptr;
    id = std::string();
}

Scope::Scope(Scope *scope, const std::string &iD) {
    parent = scope;
    id = iD;
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
