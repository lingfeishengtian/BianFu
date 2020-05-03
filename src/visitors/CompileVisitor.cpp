//
// Created by hunterhan on 5/2/20.
//
#include <iostream>
#include <string>

#include "CompileVisitor.h"
#include "../error/BianFuError.h"

std::map<std::string, llvm::FunctionCallee> CompileVisitor::systemFunctions;
std::map<std::string, llvm::Value*> CompileVisitor::systemStrings;

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
    CompileVisitor::systemFunctions["printf"] = printf;

    CompileVisitor::systemStrings["newLine"] = builder.CreateGlobalStringPtr("\n");
    CompileVisitor::systemStrings["intPrint"] = builder.CreateGlobalStringPtr("%d");
    CompileVisitor::systemStrings["doublePrint"] = builder.CreateGlobalStringPtr("%f");

    for (auto* stat : ctx->stat()){
        visitStat(stat);
    }

    builder.CreateRetVoid();

    module->print(llvm::outs(), nullptr);
    logger.log("程序汇编结束。");
    return nullptr;
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
    }else if(ctx->defaultFunctions() != nullptr){
        return visitDefaultFunctions(ctx->defaultFunctions());
    }else if(ctx->id != nullptr){
        if(scope->variables.count(ctx->id->getText())){
            llvm::AllocaInst* allocated = static_cast<llvm::AllocaInst*>(scope->variables[ctx->id->getText()]);
            llvm::LoadInst* loaded = builder.CreateLoad(allocated);
            return static_cast<llvm::Value*>(loaded);
        }else throw BianFuError("", ctx->id->getText() + " 不存在。");
    }
    return nullptr;
}

antlrcpp::Any CompileVisitor::visitAssignment(BFParser::AssignmentContext *ctx) {
    logger.log(ctx->getText() + " 是 " + "assignment.");

    try{
        logger.log(ctx->getText());
//        if(ctx->KVar() != nullptr && scope->variables.find(ctx->expr()[0]->getText()) != scope->variables.end()) throw BianFuError("", "变量已存在！");
//        if(ctx->KVar() == nullptr && scope->variables.find(ctx->expr()[0]->getText()) == scope->variables.end()) throw BianFuError("", "变量不存在！");

        auto* retVal = static_cast<llvm::Value*>(visitExpr(ctx->expr()[1]));
        llvm::AllocaInst* allocated = builder.CreateAlloca(retVal->getType());
        builder.CreateStore(retVal, allocated);
        std::string name = ctx->expr()[0]->getText();
        logger.log(name);
        scope->variables.insert(std::make_pair(name, allocated));
    } catch (std::bad_cast&) {
        logger.log("找不到" + ctx->expr()[0]->getText());
    }

    return nullptr;
}

antlrcpp::Any CompileVisitor::visitTypeDef(BFParser::TypeDefContext *ctx) {
    if(ctx->INT() != nullptr){
        logger.log(ctx->getText() + " 是 " + "int.");
        return static_cast<llvm::Value*>(llvm::ConstantInt::get(builder.getInt64Ty(), std::stoi(ctx->INT()->getText())));
    }else if(ctx->FLOAT() != nullptr){
        logger.log(ctx->getText() + " 是 " + "float.");
        return static_cast<llvm::Value*>(llvm::ConstantFP::get(builder.getDoubleTy(), std::stod(ctx->FLOAT()->getText())));
    }
}

antlrcpp::Any CompileVisitor::visitDefaultFunctions(BFParser::DefaultFunctionsContext *ctx) {
    if(ctx->FPrint() != nullptr){
        if(ctx->expr() != nullptr){
            llvm::Value* val = (visitExpr(ctx->expr()));
            std::vector<llvm::Value *> arrArgs;
            if(val->getType()->isIntegerTy()) {
                arrArgs.push_back(systemStrings["intPrint"]);
            }else if(val->getType()->isDoubleTy()){
                arrArgs.push_back(systemStrings["doublePrint"]);
            }
            //TODO: Create a printf compiler that visits the expressions and based on the expressions, create a format string. For example, 出("A" + 32) will become printf("%s%d", ${a variable}, ${int val})
            arrArgs.push_back(val);
            builder.CreateCall(CompileVisitor::systemFunctions["printf"], arrArgs);
        }else{
            builder.CreateCall(CompileVisitor::systemFunctions["printf"], systemStrings["newLine"]);
        }
    }

    return nullptr;
}
