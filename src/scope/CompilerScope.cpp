//
// Created by hunterhan on 5/2/20.
//

#include "CompilerScope.h"

CompilerScope::CompilerScope() {}

CompilerScope::CompilerScope(std::string n) {
    name = n;
}

bool CompilerScope::isGlobal() {
    return name == "";
}
