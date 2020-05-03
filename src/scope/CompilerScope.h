//
// Created by hunterhan on 5/2/20.
//

#ifndef BIANFU_COMPILERSCOPE_H
#define BIANFU_COMPILERSCOPE_H

#include <iostream>
#include <map>
#include <llvm/IR/Instructions.h>

class CompilerScope {
public:
    std::map<std::string, llvm::AllocaInst*> variables;
    std::string name;

    CompilerScope();
    explicit CompilerScope(std::string n);

    bool isGlobal();
};


#endif //BIANFU_COMPILERSCOPE_H
