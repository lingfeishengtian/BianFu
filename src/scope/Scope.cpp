//
// Created by Hunter Han on 2020-04-28.
//

#include "Scope.h"

Scope::Scope() {
    parent = nullptr;
}

Scope::Scope(Scope *scope) {
    parent = scope;
}

bool Scope::isGlobal() {
    return parent == nullptr;
}
