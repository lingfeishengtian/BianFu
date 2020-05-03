//
// Created by hunterhan on 5/2/20.
//

#include "CompilerScope.h"

CompilerScope::CompilerScope() {}

CompilerScope::CompilerScope(std::string n, CompilerScope* p) {
    name = n;
    parent = p;
}

bool CompilerScope::isGlobal() {
    return name == "";
}
