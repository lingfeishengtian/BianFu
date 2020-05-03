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
    llvm::Value *format = builder.CreateGlobalStringPtr("Value is %d!\n");
    llvm::Value *strformat = builder.CreateGlobalStringPtr("STRING print is %s!\n");

    std::vector<llvm::Type *> putsArgs;
    putsArgs.push_back(builder.getInt8Ty()->getPointerTo());
    llvm::ArrayRef<llvm::Type*>  argsRef(putsArgs);

    llvm::FunctionType *putsType =
            llvm::FunctionType::get(builder.getInt32Ty(), argsRef, true);
    llvm::FunctionCallee printf = module->getOrInsertFunction("printf", putsType);

    Type *i8 = builder.getInt8Ty();
    Constant* val = ConstantInt::get(i8, 2);
    AllocaInst* alloc = builder.CreateAlloca(i8, 0, "a");
    StoreInst* store = builder.CreateStore(val, alloc);
    LoadInst* loadedInt = builder.CreateLoad(i8, alloc);

    ArrayType *arrType = ArrayType::get(i8, 3);
    Value* char1 = ConstantInt::get(i8, 72);
    AllocaInst* arrAlloc = builder.CreateAlloca(arrType);

    std::vector<llvm::Value*> getArrArgs;
    getArrArgs.push_back(ConstantInt::get(i8, 0));
    getArrArgs.push_back(ConstantInt::get(i8, 0));

    Value* firstInd = builder.CreateInBoundsGEP(arrAlloc, ArrayRef<Value*>(getArrArgs));
    builder.CreateStore(char1, firstInd);

    std::vector<llvm::Value*> getArrArgs1;
    getArrArgs1.push_back(ConstantInt::get(i8, 0));
    getArrArgs1.push_back(ConstantInt::get(i8, 1));
    Value* secondInd = builder.CreateInBoundsGEP(arrAlloc, ArrayRef<Value*>(getArrArgs1));
    builder.CreateStore(char1, secondInd);

    std::vector<llvm::Value*> test;
    test.push_back(format);
    test.push_back(val);

    builder.CreateCall(printf, ArrayRef<Value*>(test));
    builder.CreateCall(printf, helloWorld);

    std::vector<llvm::Value*> test1;
    test1.push_back(strformat);
    test1.push_back(arrAlloc);

    builder.CreateCall(printf, ArrayRef<Value*>(test1));

    builder.CreateRetVoid();
    module->print(llvm::outs(), nullptr);

}