//
// Created by hunterhan on 5/2/20.
//
#include <iostream>
#include <string>

#include "CompileVisitor.h"
#include "../error/BianFuError.h"

static llvm::LLVMContext context;
llvm::Module *module;
llvm::IRBuilder<> builder(context);

CompileVisitor::CompileVisitor() {
    module = new llvm::Module("main", context);
    scope = new CompilerScope();
}

CompileVisitor::CompileVisitor(std::string modName, CompilerScope* s) {
    module = new llvm::Module(modName, context);
    scope = s;
}

antlrcpp::Any CompileVisitor::visitMain(BFParser::MainContext *ctx) {
    logger.log("开始汇编蝙蝠程序。");

    llvm::FunctionType *funcType = llvm::FunctionType::get(builder.getVoidTy(), false);
    llvm::Function *mainFunc =
            llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, "main", module);
    llvm::BasicBlock *entry = llvm::BasicBlock::Create(context, "entrypoint", mainFunc);
    builder.SetInsertPoint(entry);

    // Add support library!
    std::vector<llvm::Type *> printFArgs;
    printFArgs.push_back(builder.getInt8Ty()->getPointerTo());
    llvm::ArrayRef<llvm::Type*> argsRef(printFArgs);

    llvm::FunctionType *putsType =
            llvm::FunctionType::get(builder.getInt32Ty(), argsRef, true);
    llvm::FunctionCallee printf = module->getOrInsertFunction("printf", putsType);

    for (auto* stat : ctx->stat()){
        visitStat(stat);
    }

    module->print(llvm::outs(), nullptr);

    logger.log("程序汇编结束。");

    return BFParserBaseVisitor::visitMain(ctx);
}

antlrcpp::Any CompileVisitor::visitStat(BFParser::StatContext *ctx) {
    if(ctx->assignment() != nullptr){
        return visitAssignment(ctx->assignment());
    }
    return BFParserBaseVisitor::visitStat(ctx);
}

antlrcpp::Any CompileVisitor::visitExpr(BFParser::ExprContext *ctx) {
    if(ctx->typeDef() != nullptr){
        return visitTypeDef(ctx->typeDef());
    }
    return BFParserBaseVisitor::visitExpr(ctx);
}

antlrcpp::Any CompileVisitor::visitAssignment(BFParser::AssignmentContext *ctx) {
    logger.log(ctx->getText() + " 是 " + " assignment.");

    try{
        logger.log(ctx->getText());
//        if(ctx->KVar() != nullptr && scope->variables.find(ctx->expr()[0]->getText()) != scope->variables.end()) throw BianFuError("", "变量已存在！");
//        if(ctx->KVar() == nullptr && scope->variables.find(ctx->expr()[0]->getText()) == scope->variables.end()) throw BianFuError("", "变量不存在！");

        auto* retVal = static_cast<llvm::Value*>(visitExpr(ctx->expr()[1]));
        llvm::AllocaInst* allocated = builder.CreateAlloca(retVal->getType());
        builder.CreateStore(retVal, allocated);
        std::string name = ctx->expr()[0]->getText();
        logger.log(name);
        scope->variables[name] = allocated;
    } catch (std::bad_cast&) {
        logger.log("找不到" + ctx->expr()[0]->getText());
    }
}

antlrcpp::Any CompileVisitor::visitTypeDef(BFParser::TypeDefContext *ctx) {
    if(ctx->INT() != nullptr){
        logger.log(ctx->getText() + " 是 " + " int.");
        return static_cast<llvm::Value*>(llvm::ConstantInt::get(builder.getInt64Ty(), std::stoi(ctx->INT()->getText())));
    }
}
