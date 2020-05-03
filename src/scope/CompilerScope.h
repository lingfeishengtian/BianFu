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
    CompilerScope* parent;

    std::map<std::string, llvm::Value*> variables;
    std::map<std::string, llvm::Function> functions;
    std::string name;

    CompilerScope();
    explicit CompilerScope(std::string n, CompilerScope* p);

    bool isGlobal();
};


#endif //BIANFU_COMPILERSCOPE_H
