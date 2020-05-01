#include <iostream>

#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Verifier.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/ExecutionEngine/SectionMemoryManager.h"
#include "llvm/ExecutionEngine/Orc/CompileUtils.h"
#include "llvm/Support/TargetSelect.h"

// Optimizations
#include "llvm/Transforms/Scalar.h"
#include "llvm/Analysis/BasicAliasAnalysis.h"

using namespace llvm;

int main()
{
    static llvm::LLVMContext context;
    llvm::Module *module = new llvm::Module("asdf", context);
    llvm::IRBuilder<> builder(context);

    llvm::FunctionType *funcType = llvm::FunctionType::get(builder.getVoidTy(), false);
    llvm::Function *mainFunc =
            llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, "main", module);
    llvm::BasicBlock *entry = llvm::BasicBlock::Create(context, "entrypoint", mainFunc);
    builder.SetInsertPoint(entry);

    llvm::Value *helloWorld = builder.CreateGlobalStringPtr("hello world!");

    std::vector<llvm::Type *> putsArgs;
    putsArgs.push_back(builder.getInt8Ty()->getPointerTo());
    llvm::ArrayRef<llvm::Type*>  argsRef(putsArgs);

    llvm::FunctionType *putsType =
            llvm::FunctionType::get(builder.getInt32Ty(), argsRef, false);
    llvm::FunctionCallee putsFunc = module->getOrInsertFunction("printf", putsType);

    Type *i8 = builder.getInt8Ty();
    Value* val = ConstantInt::get(i8, 2);
    AllocaInst* alloc = builder.CreateAlloca(i8, 0, "a");
    StoreInst* store = builder.CreateStore(val, alloc);

    builder.CreateCall(putsFunc, helloWorld);
    builder.CreateCall(putsFunc, builder.CreateLoad(alloc, ""));
    builder.CreateRetVoid();
    module->print(llvm::outs(), nullptr);

}