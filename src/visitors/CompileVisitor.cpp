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
    CompileVisitor::systemStrings["strPrint"] = builder.CreateGlobalStringPtr("%s");

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
            logger.log(ctx->getText());
            llvm::AllocaInst* allocated = static_cast<llvm::AllocaInst*>(scope->variables[ctx->id->getText()]);
            llvm::Type* allocatedType = allocated->getAllocatedType();
            if(allocatedType->isIntegerTy() || allocatedType->isDoubleTy()) {
                    llvm::LoadInst *loaded = builder.CreateLoad(allocatedType, allocated);
                    return static_cast<llvm::Value *>(loaded);
            }else{
                return static_cast<llvm::Value *>(allocated);
            }
        }else throw BianFuError("", ctx->id->getText() + " 不存在。");
    }else if(ctx->op != nullptr){
        llvm::Value* first = visitExpr(ctx->expr()[0]);
        llvm::Value* second = visitExpr(ctx->expr()[1]);

        llvm::Type* typeF = first->getType();
        llvm::Type* typeS = second->getType();

        if(typeF->isDoubleTy() && typeS->isIntegerTy()){
            llvm::Value* casted = builder.CreateSIToFP(second, builder.getDoubleTy());
            return opDoubles(first, casted, ctx->op->getText());
        }else if(typeF->isIntegerTy() && typeS->isDoubleTy()){
            llvm::Value* casted = builder.CreateSIToFP(first, builder.getDoubleTy());
            return opDoubles(casted, second, ctx->op->getText());
        }else{

        }
    }else if(ctx->array() != nullptr){
        llvm::AllocaInst* allocaInst = visitArray(ctx->array());
        return allocaInst;
    }
    return nullptr;
}

llvm::Value *CompileVisitor::opDoubles(llvm::Value *f, llvm::Value *s, std::string op) {
    if(op == "+"){
        return builder.CreateFAdd(f, s);
    }else if(op == "-"){
        return builder.CreateFSub(f, s);
    }
    return nullptr;
}

antlrcpp::Any CompileVisitor::visitAssignment(BFParser::AssignmentContext *ctx) {
    logger.log(ctx->getText() + " 是 " + "assignment.");

    try{
        logger.log(ctx->getText());
//        if(ctx->KVar() != nullptr && scope->variables.find(ctx->expr()[0]->getText()) != scope->variables.end()) throw BianFuError("", "变量已存在！");
//        if(ctx->KVar() == nullptr && scope->variables.find(ctx->expr()[0]->getText()) == scope->variables.end()) throw BianFuError("", "变量不存在！");

        llvm::AllocaInst* allocated;
        antlrcpp::Any ret = visitExpr(ctx->expr()[1]);
        try {
            llvm::AllocaInst* allocaInst = ret.as<llvm::AllocaInst*>();
            allocated = allocaInst;
        } catch (std::bad_cast&) {
            llvm::Value* val = ret.as<llvm::Value*>();
            allocated = builder.CreateAlloca(val->getType());
            builder.CreateStore(val, allocated);
        }
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
    }else if(ctx->String() != nullptr){
        logger.log(ctx->getText() + " 是 " + "字母.");

    }else if(ctx->Char() != nullptr){
        if(ctx->Char()->getText().size() > 3) throw BianFuError("", "字太大了！");
        logger.log(ctx->getText() + " 是 " + "字.");
        return static_cast<llvm::Value*>(llvm::ConstantInt::get(builder.getInt8Ty(), ctx->Char()->getText().at(1)));
    }
}

antlrcpp::Any CompileVisitor::visitDefaultFunctions(BFParser::DefaultFunctionsContext *ctx) {
    if(ctx->FPrint() != nullptr){
        //TODO: Learn how to concatenate strings! *hint: strcat with c api
        if(ctx->expr() != nullptr){
            std::vector<llvm::Value *> arrArgs;
            try {
                llvm::Value *val = (visitExpr(ctx->expr()));
                if (val->getType()->isIntegerTy()) {
                    arrArgs.push_back(systemStrings["intPrint"]);
                } else if (val->getType()->isDoubleTy()) {
                    arrArgs.push_back(systemStrings["doublePrint"]);
                }else{
                    arrArgs.push_back(systemStrings["strPrint"]);
                }
                arrArgs.push_back(val);
            } catch (std::bad_cast&) {
                arrArgs.push_back(systemStrings["strPrint"]);
                arrArgs.push_back(static_cast<llvm::AllocaInst*>(visitExpr(ctx->expr())));
            }
            builder.CreateCall(CompileVisitor::systemFunctions["printf"], arrArgs);
        }else{
            builder.CreateCall(CompileVisitor::systemFunctions["printf"], systemStrings["newLine"]);
        }
    }

    return nullptr;
}

antlrcpp::Any CompileVisitor::visitClassStat(BFParser::ClassStatContext *ctx) {
    return BFParserBaseVisitor::visitClassStat(ctx);
}

antlrcpp::Any CompileVisitor::visitClassDeclaration(BFParser::ClassDeclarationContext *ctx) {
    return BFParserBaseVisitor::visitClassDeclaration(ctx);
}

antlrcpp::Any CompileVisitor::visitFunctionDeclaration(BFParser::FunctionDeclarationContext *ctx) {
    return BFParserBaseVisitor::visitFunctionDeclaration(ctx);
}

antlrcpp::Any CompileVisitor::visitBlock(BFParser::BlockContext *ctx) {
    return BFParserBaseVisitor::visitBlock(ctx);
}

antlrcpp::Any CompileVisitor::visitArray(BFParser::ArrayContext *ctx) {
    logger.log(ctx->getText() + " is array");
    llvm::Type* finType = builder.getInt8Ty();
    llvm::Type* arrIndTy = builder.getInt8Ty();

    std::vector<llvm::Value*> valuesRetrieved;
    for (auto* expr : ctx->expr()){
        llvm::Value* expressionVisited = visitExpr(expr);
        valuesRetrieved.push_back(expressionVisited);

            //TODO: Search scope for most closely related relative
        if (expressionVisited->getType()->isDoubleTy()) {
            finType = builder.getDoubleTy();
        } else if (expressionVisited->getType()->isIntegerTy(64) && !expressionVisited->getType()->isDoubleTy()){
            finType = builder.getInt64Ty();
        }
    }

    llvm::ArrayType* arrType = llvm::ArrayType::get(finType, ctx->expr().size());
    llvm::AllocaInst* arrAlloc = builder.CreateAlloca(arrType);

    for (int i = 0; i < valuesRetrieved.size(); i++){
        llvm::Value* expressionVisited = valuesRetrieved[i];

        std::vector<llvm::Value*> getArrArgs;
        getArrArgs.push_back(llvm::ConstantInt::get(arrIndTy, 0));
        getArrArgs.push_back(llvm::ConstantInt::get(arrIndTy, i));

        llvm::Value *indexPointer = builder.CreateInBoundsGEP(arrAlloc, llvm::ArrayRef<llvm::Value *>(getArrArgs));
        if(ctx->expr()[i]->typeDef() != nullptr) {
            expressionVisited->mutateType(finType);
            builder.CreateStore(expressionVisited, indexPointer);
        }else if(expressionVisited->getType()->isIntegerTy()){
            builder.CreateStore(builder.CreateIntCast(expressionVisited, finType, false), indexPointer);
        }else{
            builder.CreateStore(expressionVisited, indexPointer);
        }
    }

    return arrAlloc;
}
